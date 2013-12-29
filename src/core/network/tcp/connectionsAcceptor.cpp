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

ConnectionsAcceptor::ConnectionsAcceptor(asio::io_service &ioService, uint16_t port, Protocol &protocol):
    acceptor_(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    strand_(ioService),
    protocol_(protocol) {}

ConnectionsAcceptor::~ConnectionsAcceptor() {}

void ConnectionsAcceptor::queueAccept()
{
    Connection::Pointer connection = Connection::Pointer(new Connection(acceptor_.get_io_service(), protocol_));

    acceptor_.async_accept(connection->getSocket(),
                           strand_.wrap(std::bind(&ConnectionsAcceptor::acceptHandler,
                                                  this,
                                                  connection,
                                                  std::placeholders::_1)));
}

void ConnectionsAcceptor::acceptHandler(Connection::Pointer connection, const boost::system::error_code &errorCode)
{
    if(!errorCode)
    {
        connection->accept();
    }
    else if(errorCode != boost::asio::error::operation_aborted)
    {
        LOG(ERROR) << "Error during establishing connection, error: " << errorCode.message();
    }
    else
    {
        return;
    }

    this->queueAccept();
}

asio::ip::tcp::acceptor& ConnectionsAcceptor::getAcceptor()
{
    return acceptor_;
}

}
}
}
}
