#pragma once

#include <core/network/udp/writeBufferFactory.hpp>
#include <common/asio.hpp>

#include <boost/noncopyable.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace udp
{

class Connection: private boost::noncopyable
{
public:
    typedef std::function<void(Connection&, const boost::asio::ip::udp::endpoint&)> ReceiveFromEventCallback;
    typedef std::shared_ptr<asio::ip::udp::socket> SocketPointer;
    typedef std::shared_ptr<Connection> Pointer;

    Connection(asio::io_service &ioService, uint16_t port);
    explicit Connection(SocketPointer socket);
    virtual ~Connection();

    Payload& getReadPayload();
    void setReceiveFromEventCallback(const ReceiveFromEventCallback &callback);
    void queueReceiveFrom();
    void sendTo(const boost::asio::ip::udp::endpoint &endpoint, const Payload &payload);

    void receiveFromHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);
    void sendToHandler(const boost::asio::ip::udp::endpoint &endpoint, const boost::system::error_code &errorCode, size_t bytesTransferred);

private:
    Connection();

    void queueSendTo(const boost::asio::ip::udp::endpoint &endpoint, WriteBuffer &writeBuffer);
    void errorHandler(const boost::system::error_code &errorCode, const std::string &operationName);

    SocketPointer socket_;
    asio::io_service::strand strand_;
    Payload readPayload_;
    WriteBufferFactory writeBufferFactory_;
    boost::asio::ip::udp::endpoint senderEndpoint_;
    ReceiveFromEventCallback receiveFromEventCallback_;
};

}
}
}
}
