#include <analyzer/user.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace analyzer
{

User::User(core::network::tcp::Connection::Pointer connection):
    NetworkUser(connection) {}

void User::storeReadPayload()
{
    readPayloads_.push_back(connection_->getReadPayload());
}

std::list<core::network::Payload> &User::getReadPayloads()
{
    return readPayloads_;
}

}
}
