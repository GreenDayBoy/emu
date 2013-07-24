#include <glog/logging.h>
#include <core/network/tcp/connection.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

Connection::Connection(SocketPointer socket):
    socket_(socket),
    strand_(socket_->get_io_service()),
    closeOngoing_(false) {}

Connection::~Connection() {}

ReadBuffer& Connection::readBuffer()
{
    return readBuffer_;
}

void Connection::setConnectEventCallback(const EventCallback &callback)
{
    connectEventCallback_ = callback;
}

void Connection::setReceiveEventCallback(const EventCallback &callback)
{
    receiveEventCallback_ = callback;
}

void Connection::setCloseEventCallback(const EventCallback &callback)
{
    closeEventCallback_ = callback;
}

void Connection::disconnect()
{
    if(socket_->is_open())
    {
        closeOngoing_ = true;

        asio::io_service &service = socket_->get_io_service();
        service.post(strand_.wrap(std::bind(closeEventCallback_, std::ref(*this))));
    }
}

void Connection::close()
{
    socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socket_->close();
}

void Connection::send(const Payload &payload)
{
    bool result = writeBuffer_.insert(payload);

    if(!result)
    {
        this->errorHandler(boost::asio::error::no_buffer_space, "send");
        return;
    }

    if(!writeBuffer_.pending_)
    {
        writeBuffer_.pending_ = true;
        this->queueSend();
    }
}

void Connection::queueReceive()
{
    socket_->async_receive(boost::asio::buffer(&readBuffer_.payload_[0], kMaxPayloadSize),
                           strand_.wrap(std::bind(&Connection::receiveHandler,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2)));
}

void Connection::connect(const boost::asio::ip::tcp::endpoint &endpoint)
{
    socket_->async_connect(endpoint, std::bind(&Connection::connectHandler,
                           this,
                           std::placeholders::_1));
}

void Connection::queueSend()
{
    socket_->async_send(boost::asio::buffer(&writeBuffer_.payload_[0], writeBuffer_.payloadSize_),
                        strand_.wrap(std::bind(&Connection::sendHandler,
                                     this,
                                     std::placeholders::_1,
                                     std::placeholders::_2)));
}

void Connection::receiveHandler(const boost::system::error_code &errorCode, size_t bytesTransferred)
{
    if(boost::asio::error::eof == errorCode)
    {
        this->disconnect();
        return;
    }
    else if(errorCode)
    {
        this->errorHandler(errorCode, "receive");
        return;
    }

    readBuffer_.payloadSize_ = bytesTransferred;
    receiveEventCallback_(*this);

    if(!closeOngoing_)
    {
        this->queueReceive();
    }
}

void Connection::sendHandler(const boost::system::error_code& errorCode, size_t bytesTransferred)
{
    if(errorCode)
    {
        this->errorHandler(errorCode, "send");
        return;
    }

    if(writeBuffer_.secondPayloadSize_ > 0)
    {
        writeBuffer_.swap();
        this->queueSend();
    }
    else
        writeBuffer_.clear();
}

void Connection::errorHandler(const boost::system::error_code &errorCode, const std::string &operationName)
{
    if(boost::asio::error::operation_aborted == errorCode)
    {
        return;
    }

    LOG(ERROR) << "Error during handling async operation: " << operationName
               << ", error: " << errorCode.message()
               << ", code: " << errorCode.value();

    this->disconnect();
}

void Connection::connectHandler(const boost::system::error_code &errorCode)
{
    if(errorCode)
    {
        this->errorHandler(errorCode, "connect");
        return;
    }

    connectEventCallback_(*this);
}

std::ostream& operator<<(std::ostream &stream, const Connection &connection)
{
    stream << "address: " << connection.socket_->remote_endpoint().address().to_string();

    return stream;
}

}
}
}
}
