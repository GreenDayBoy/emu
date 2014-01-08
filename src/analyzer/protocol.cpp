#include <analyzer/protocol.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace analyzer
{

Protocol::Protocol(Controller &controller):
    controller_(controller) {}

bool Protocol::attach(core::network::tcp::Connection::Pointer connection)
{
    if(controller_.getUsersFactory().getObjects().size() > controller_.getMaxNumberOfUsers())
    {
        eMU_LOG(warning) << "Max number of users reached.";
        return false;
    }

    User &user = controller_.getUsersFactory().create(connection);

    eMU_LOG(info) << "User connected, hash: " << user.getHash();

    controller_.onAccept(user);

    return true;
}

void Protocol::detach(core::network::tcp::Connection::Pointer connection)
{
    try
    {
        User &user = controller_.getUsersFactory().find(connection);

        eMU_LOG(info) << "User closed, hash: " << user.getHash();

        controller_.onClose(user);
        controller_.getUsersFactory().destroy(user);

        connection->close();
    }
    catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
    {
        eMU_LOG(error) << "could not find user by connection.";
        connection->close();
    }
}

bool Protocol::dispatch(core::network::tcp::Connection::Pointer connection)
{
    try
    {
        User &user = controller_.getUsersFactory().find(connection);

        eMU_LOG(info) << "Received, hash: " << user.getHash() << ", #bytes: " << user.getConnection().getReadPayload().getSize();

        user.storeReadPayload();
        controller_.onReceive(user);
    }
    catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
    {
        eMU_LOG(error) << "could not find user by connection.";
        return false;
    }

    return true;
}

}
}
