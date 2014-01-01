#include <protocols/contexts/client.hpp>

namespace eMU
{
namespace protocols
{
namespace contexts
{

Client::Client():
    clientConnection_(nullptr) {}

Client::~Client() {}

void Client::setClientConnection(core::network::tcp::Connection::Pointer connection)
{
    clientConnection_ = connection;
}

core::network::tcp::Connection::Pointer Client::getClientConnection()
{
    return clientConnection_;
}

}
}
}
