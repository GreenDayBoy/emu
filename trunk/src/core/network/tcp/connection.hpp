#pragma once

#ifdef eMU_UT
#include <ut/env/asioStub/ioService.hpp>
#include <ut/env/asioStub/tcp/socket.hpp>
#elif eMU_MT
#include <mt/env/asioStub/ioService.hpp>
#include <mt/env/asioStub/tcp/socket.hpp>
#else
#include <boost/asio.hpp>
#endif

#include <boost/noncopyable.hpp>
#include <functional>

#include <network/buffer.hpp>
#include <common/hashableObject.hpp>


namespace eMU {
namespace core {
namespace network {
namespace tcp {

#ifdef eMU_UT
namespace asio = eMU::ut::env::asioStub;
#elif eMU_MT
namespace asio = eMU::mt::env::asioStub;
#else
namespace asio = boost::asio;
#endif

class Connection: boost::noncopyable, public common::HashableObject {
public:
    typedef std::function<void(Connection&)> EventCallback;
    typedef std::shared_ptr<Connection> Pointer;

    Connection(asio::io_service &ioService);
    virtual ~Connection();

    ReadBuffer& readBuffer();
    std::string address() const;

    void setConnectEventCallback(const EventCallback &callback);
    void setReceiveEventCallback(const EventCallback &callback);
    void setCloseEventCallback(const EventCallback &callback);

    void disconnect();
    void close();
    void send(const Payload &payload);
    void queueReceive();
    void connect(const boost::asio::ip::tcp::endpoint &endpoint);
    asio::ip::tcp::socket& socket();

private:
    Connection();

    void queueSend();

    void receiveHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);
    void sendHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);
    void errorHandler(const boost::system::error_code &errorCode, const std::string &operationName);
    void connectHandler(const boost::system::error_code &errorCode);

    asio::ip::tcp::socket socket_;

    ReadBuffer readBuffer_;
    WriteBuffer writeBuffer_;

    asio::io_service::strand strand_;
    bool closeOngoing_;

    EventCallback connectEventCallback_;
    EventCallback receiveEventCallback_;
    EventCallback closeEventCallback_;
};

}
}
}
}
