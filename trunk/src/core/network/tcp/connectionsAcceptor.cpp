#include <core/network/tcp/connectionsAcceptor.hpp>
#include <glog/logging.h>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

ConnectionsAcceptor::ConnectionsAcceptor(common::Factory<Connection> &connectionsFactory, AcceptorPointer acceptor):
    connectionsFactory_(connectionsFactory),
    acceptor_(acceptor),
    strand_(acceptor_->get_io_service()) {}

ConnectionsAcceptor::~ConnectionsAcceptor() {}

void ConnectionsAcceptor::queueAccept()
{
    Connection::SocketPointer socket(new asio::ip::tcp::socket(acceptor_->get_io_service()));

    acceptor_->async_accept(*socket,
                            strand_.wrap(std::bind(&ConnectionsAcceptor::acceptHandler,
                                                   this,
                                                   socket,
                                                   std::placeholders::_1)));
}

void ConnectionsAcceptor::acceptHandler(Connection::SocketPointer socket, const boost::system::error_code &errorCode)
{
    if(errorCode)
    {
        LOG(ERROR) << "Error during establishing connection, error: " << errorCode.message();
    }
    else
    {
        Connection &connection = connectionsFactory_.create(socket);
        acceptEventCallback_(connection);
    }

    this->queueAccept();
}

void ConnectionsAcceptor::setAcceptEventCallback(const AcceptEventCallback &callback)
{
    acceptEventCallback_ = callback;
}

}
}
}
}
