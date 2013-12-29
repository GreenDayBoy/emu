#include <analyzer/protocol.hpp>

#include <glog/logging.h>

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
        LOG(WARNING) << "Max number of users reached.";
        return false;
    }

    User &user = controller_.getUsersFactory().create(connection);

    LOG(INFO) << "hash: " << user.getHash() <<  ", user registered.";

    controller_.onAccept(user);

    return true;
}

void Protocol::detach(core::network::tcp::Connection::Pointer connection)
{
    try
    {
        User &user = controller_.getUsersFactory().find(connection);

        LOG(INFO) << "hash: " << user.getHash() <<  ", user closed.";

        controller_.onClose(user);

        controller_.getUsersFactory().destroy(user);
        connection->close();
    }
    catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "user not found!";
        connection->close();
    }
}

bool Protocol::dispatch(core::network::tcp::Connection::Pointer connection)
{
    try
    {
        User &user = controller_.getUsersFactory().find(connection);

        LOG(INFO) << "hash: " << user.getHash() << ", received bytes: " << user.getConnection().getReadPayload().getSize();

        user.storeReadPayload();
        controller_.onReceive(user);
    }
    catch(const typename core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "user not found!";
        return false;
    }

    return true;
}

}
}
