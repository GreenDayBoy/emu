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

Connection::Connection(asio::io_service &ioService):
    Connection(SocketPointer(new asio::ip::tcp::socket(ioService))) {}

Connection::Connection(SocketPointer socket):
    socket_(socket),
    strand_(socket_->get_io_service()),
    closeOngoing_(false) {}

Connection::~Connection() {}

Payload& Connection::getReadPayload()
{
    return readPayload_;
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
    if(isOpen())
    {
        closeOngoing_ = true;

        asio::io_service &service = socket_->get_io_service();
        service.post(strand_.wrap(std::bind(closeEventCallback_, std::ref(*this))));
    }
}

void Connection::close()
{
    try
    {
        socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_->close();
    }
    catch(const boost::system::system_error &error)
    {
        LOG(ERROR) << "Error during closing socket, error: " << error.what()
                   << ", code: " << error.code();
    }
}

void Connection::send(const Payload &payload)
{
    bool result = writeBuffer_.insert(payload);

    if(!result)
    {
        this->errorHandler(boost::asio::error::no_buffer_space, "send");
        return;
    }

    if(!writeBuffer_.isPending())
    {
        writeBuffer_.setPendingState();
        this->queueSend();
    }
}

void Connection::queueReceive()
{
    socket_->async_receive(boost::asio::buffer(&readPayload_[0], Payload::getMaxSize()),
                           strand_.wrap(std::bind(&Connection::receiveHandler,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2)));
}

bool Connection::connect(const boost::asio::ip::tcp::endpoint &endpoint)
{
    boost::system::error_code errorCode;
    socket_->connect(endpoint, errorCode);

    if(errorCode)
    {
        LOG(ERROR) << "Error during connect, address: " << endpoint.address().to_string() << ", port: " <<  endpoint.port()
                   << ", error: " << errorCode.message() << ", code: " << errorCode.value();

        if(socket_->is_open())
        {
            this->close();
        }

        return false;
    }

    this->queueReceive();
    return true;
}

void Connection::queueSend()
{
    socket_->async_send(boost::asio::buffer(&writeBuffer_.getPayload()[0], writeBuffer_.getPayload().getSize()),
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

    readPayload_.setSize(bytesTransferred); // we should trust ASIO and belive that bytesTransfered never will be greater than maxSize
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

    if(writeBuffer_.getSecondPayload().getSize() > 0)
    {
        writeBuffer_.swap();
        this->queueSend();
    }
    else
        writeBuffer_.clear();
}

void Connection::errorHandler(const boost::system::error_code &errorCode, const std::string &operationName)
{
    if(boost::asio::error::operation_aborted == errorCode ||
       boost::asio::error::connection_reset == errorCode)
    {
        return;
    }

    LOG(ERROR) << "Error during handling async operation: " << operationName
               << ", error: " << errorCode.message()
               << ", code: " << errorCode.value();

    this->disconnect();
}

bool Connection::isOpen() const
{
    return socket_->is_open();
}

bool Connection::operator==(const Connection &connection) const
{
    return reinterpret_cast<size_t>(this) == reinterpret_cast<size_t>(&connection);
}

}
}
}
}
