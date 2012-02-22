#ifndef eMUCORE_SERVER_HPP
#define eMUCORE_SERVER_HPP

#include <boost/asio.hpp>
#include "log.hpp"
#include "serverEntity.hpp"

namespace eMUCore {
namespace network {

template<typename Service = boost::asio::io_service,
         typename Acceptor = boost::asio::ip::tcp::acceptor,
         typename SocketImpl = socket_t<> >
class server_t: private boost::noncopyable {
public:
    server_t(Service &ioService,
             serverEntity_t<SocketImpl> &serverEntity):
      ioService_(ioService),
      acceptor_(ioService),
      serverEntity_(serverEntity) {}
    virtual ~server_t() {}

    virtual void initialize() {
        this->queueAccept();
    }

    virtual void cleanup() {
    }

protected:
    virtual void queueAccept() {
        SocketImpl::ptr_t socket(new SocketImpl(ioService_,
                                                boost::bind(&serverEntity_t<SocketImpl>::onPeerReceive,
                                                            &serverEntity_,
                                                            _1),
                                                boost::bind(&serverEntity_t<SocketImpl>::onPeerClose,
                                                            &serverEntity_,
                                                            _1)));

        acceptor_.async_accept(socket->impl(),
                               boost::bind(&server_t::acceptHandler,
                                           this,
                                           socket,
                                           boost::asio::placeholders::error));
    }

    virtual void acceptHandler(typename SocketImpl::ptr_t socket,
                               const boost::system::error_code &ec) {
        if(!ec) {
            serverEntity_.onPeerConnect(socket);
        } else {
            LOG_ERROR << "Error during establishing connection, error: " << ec.message() << std::endl;
        }
        
        this->queueAccept();
    }

    Service &ioService_;
    Acceptor acceptor_;
    serverEntity_t<SocketImpl> &serverEntity_;

private:
    server_t();
};

}
}

#endif