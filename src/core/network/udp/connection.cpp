#include <core/network/udp/connection.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace core
{
namespace network
{
namespace udp
{

Connection::Connection(asio::io_service &ioService, uint16_t port):
    Connection(SocketPointer(new asio::ip::udp::socket(ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)))) {}


Connection::Connection(SocketPointer socket):
    socket_(socket),
    strand_(socket_->get_io_service()) {}

Connection::~Connection() {}

Payload& Connection::getReadPayload()
{
    return readPayload_;
}

void Connection::setReceiveFromEventCallback(const ReceiveFromEventCallback &callback)
{
    receiveFromEventCallback_ = callback;
}

void Connection::queueReceiveFrom()
{
    socket_->async_receive_from(boost::asio::buffer(&readPayload_[0], Payload::getMaxSize()),
                                senderEndpoint_,
                                strand_.wrap(std::bind(&Connection::receiveFromHandler,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2)));
}

void Connection::sendTo(const boost::asio::ip::udp::endpoint &endpoint, const Payload &payload)
{
    WriteBuffer &writeBuffer = writeBufferFactory_.get(endpoint);

    bool result = writeBuffer.insert(payload);

    if(!result)
    {
        this->errorHandler(boost::asio::error::no_buffer_space, "sendTo");
        return;
    }

    if(!writeBuffer.isPending())
    {
        writeBuffer.setPendingState();
        this->queueSendTo(endpoint, writeBuffer);
    }
}

void Connection::receiveFromHandler(const boost::system::error_code &errorCode, size_t bytesTransferred)
{
    if(errorCode)
    {
        this->errorHandler(errorCode, "receiveFrom");
    }
    else
    {
        readPayload_.setSize(bytesTransferred); // we should trust ASIO and belive that bytesTransfered never will be greater than maxSize
        receiveFromEventCallback_(*this, senderEndpoint_);
    }

    this->queueReceiveFrom();
}

void Connection::queueSendTo(const boost::asio::ip::udp::endpoint &endpoint, WriteBuffer &writeBuffer)
{
    socket_->async_send_to(boost::asio::buffer(&writeBuffer.getPayload()[0], writeBuffer.getPayload().getSize()),
                           endpoint,
                           strand_.wrap(std::bind(&Connection::sendToHandler,
                                        this,
                                        endpoint,
                                        std::placeholders::_1,
                                        std::placeholders::_2)));
}

void Connection::sendToHandler(const boost::asio::ip::udp::endpoint &endpoint, const boost::system::error_code& errorCode, size_t bytesTransferred)
{
    if(errorCode)
    {
        this->errorHandler(errorCode, "sendTo");
    }

    WriteBuffer &writeBuffer = writeBufferFactory_.get(endpoint);

    if(writeBuffer.getSecondPayload().getSize() > 0)
    {
        writeBuffer.swap();
        this->queueSendTo(endpoint, writeBuffer);
    }
    else
    {
        writeBufferFactory_.erase(endpoint);
    }
}

void Connection::errorHandler(const boost::system::error_code &errorCode, const std::string &operationName)
{
    LOG(ERROR) << "Error during handling async operation: " << operationName
               << ", error: " << errorCode.message()
               << ", code: " << errorCode.value() << std::endl;
}

}
}
}
}
