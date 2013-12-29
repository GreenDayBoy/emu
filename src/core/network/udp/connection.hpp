#pragma once

#include <core/network/udp/writeBufferFactory.hpp>
#include <core/network/udp/protocol.hpp>
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
    Connection(asio::io_service &ioService, uint16_t port, Protocol &protocol);
    virtual ~Connection();

    Payload& getReadPayload();
    void queueReceiveFrom();
    void sendTo(const boost::asio::ip::udp::endpoint &endpoint, const Payload &payload);

    void receiveFromHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);
    void sendToHandler(const boost::asio::ip::udp::endpoint &endpoint, const boost::system::error_code &errorCode, size_t bytesTransferred);

    asio::ip::udp::socket& getSocket();

private:
    Connection();

    void queueSendTo(const boost::asio::ip::udp::endpoint &endpoint, WriteBuffer &writeBuffer);
    void errorHandler(const boost::system::error_code &errorCode, const std::string &operationName);

    asio::ip::udp::socket socket_;
    asio::io_service::strand strand_;
    Payload readPayload_;
    WriteBufferFactory writeBufferFactory_;
    boost::asio::ip::udp::endpoint senderEndpoint_;
    Protocol &protocol_;
};

}
}
}
}
