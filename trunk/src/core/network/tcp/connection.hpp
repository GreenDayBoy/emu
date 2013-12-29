#pragma once

#include <core/network/writeBuffer.hpp>
#include <common/mockable.hpp>
#include <common/asio.hpp>

#include <boost/noncopyable.hpp>
#include <functional>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

class Protocol;

class Connection: boost::noncopyable, public std::enable_shared_from_this<Connection>
{
public:
    typedef std::shared_ptr<Connection> Pointer;

    Connection(asio::io_service &ioService, Protocol& protocol);
    virtual ~Connection();

    Payload& getReadPayload();
    void accept();

    MOCKABLE void disconnect();
    MOCKABLE void close();
    MOCKABLE void send(const Payload &payload);
    MOCKABLE void queueReceive();
    MOCKABLE bool connect(const boost::asio::ip::tcp::endpoint &endpoint);
    MOCKABLE bool isOpen() const;

    asio::ip::tcp::socket& getSocket();

    void receiveHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);
    void sendHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);

    bool operator==(const Connection &connection) const;

private:
    Connection();

    void queueSend();
    void errorHandler(const boost::system::error_code &errorCode, const std::string &operationName);

    Protocol &protocol_;
    asio::ip::tcp::socket socket_;

    Payload readPayload_;
    WriteBuffer writeBuffer_;

    asio::io_service::strand strand_;
    bool closeOngoing_;
};

}
}
}
}
