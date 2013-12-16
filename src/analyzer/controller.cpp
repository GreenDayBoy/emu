#include <analyzer/controller.hpp>
#include <boost/lexical_cast.hpp>

#include <sstream>

namespace eMU
{
namespace analyzer
{

Controller::Controller(asio::io_service &ioService):
    server_(ioService)
{
    server_.setController(this);
    view_.setController(this);
}

Server& Controller::getServer()
{
    return server_;
}

View& Controller::getView()
{
    return view_;
}

void Controller::onAccept(User &user)
{
    view_.displayUser(boost::lexical_cast<std::string>(user.getHash()));
}

void Controller::onReceive(User &user)
{
    std::vector<std::string> streamHashes;

    for(const auto& stream: user.getReadStreams())
    {
        size_t streamHash = reinterpret_cast<size_t>(&stream);
        streamHashes.push_back(boost::lexical_cast<std::string>(streamHash));
    }

    view_.displayUserReadStreams(boost::lexical_cast<std::string>(user.getHash()), streamHashes);
}

void Controller::onClose(User &user)
{
    view_.removeUserFromDisplay(boost::lexical_cast<std::string>(user.getHash()));
}

void Controller::loadReadStream(const std::string &userId, const std::string &streamId)
{
    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(userId));
        User &user = server_.getUsersFactory().find(userHash);

        for(const auto &stream: user.getReadStreams())
        {
            size_t streamHash = reinterpret_cast<size_t>(&stream);

            if(streamId == boost::lexical_cast<std::string>(streamHash))
            {
                std::stringstream streamHex; streamHex << stream;
                view_.displayReadStream(streamHex.str(), stream.getId(), stream.getSize());
                break;
            }
        }
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find userId: " << userId;
    }
}

void Controller::disconnectUser(const std::string &userId)
{
    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(userId));
        User &user = server_.getUsersFactory().find(userHash);

        user.getConnection().disconnect();
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find userId: " << userId;
    }
}

}
}
