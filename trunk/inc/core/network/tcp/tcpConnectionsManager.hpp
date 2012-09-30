#pragma once

#include <common/objectsFactory.hpp>
#include <network/tcp/tcpConnection.hpp>

#ifdef eMU_UT
#include <ut/env/core/ioServiceStub.hpp>
#include <ut/env/core/tcp/acceptorMock.hpp>
#endif

namespace eMU {
namespace core {
namespace network {
namespace tcp {

#ifdef eMU_UT
template<typename ioServiceImpl = eMU::ut::env::core::ioServiceStub_t,
         typename acceptorImpl = eMU::ut::env::core::tcp::acceptorMock_t>
#else
template<typename ioServiceImpl = boost::asio::io_service,
         typename acceptorImpl = boost::asio::ip::tcp::acceptor>
#endif
class connectionsManager_t: public objectsFactory_t<connection_t<> > {
public:
    typedef boost::function1<void, connection_t<>& > eventCallback_t;

    connectionsManager_t(ioServiceImpl &ioService, int16 port):
      ioService_(ioService),
      acceptor_(ioService_,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

    void acceptEventCallback(const eventCallback_t &callback) { acceptEventCallback_ = callback; }
    acceptorImpl& acceptor() { return acceptor_; }

    void queueAccept() {
        connection_t<> *connection = objectsPool_.construct(boost::ref(ioService_));

        if(NULL == connection) {
            LOG_ERROR << "Error in allocating new connection object!" << std::endl;
            return;
        }

        acceptor_.async_accept(connection->socket(),
                               boost::bind(&connectionsManager_t::acceptHandler,
                                           this,
                                           boost::ref(*connection),
                                           boost::asio::placeholders::error));
    }

    void release(connection_t<> &connection) {
        connection.close();

        if(objectsPool_.is_from(&connection)) {
            objectsPool_.destroy(&connection);
        }
    }

protected:
    connectionsManager_t();

    void acceptHandler(connection_t<> &connection, const boost::system::error_code &ec) {
        if(!ec) {
            acceptEventCallback_(connection);
        } else {
            LOG_ERROR << "Error during establishing connection, error: " << ec.message() << std::endl;

            objectsPool_.destroy(&connection);
        }

        this->queueAccept();
    }

    ioServiceImpl &ioService_;
    acceptorImpl acceptor_;
    eventCallback_t acceptEventCallback_;
};

}
}
}
}
