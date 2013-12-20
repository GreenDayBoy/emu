#include <analyzer/controller.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>
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
    view_.emitConnectionAccepted(boost::lexical_cast<std::string>(user.getHash()));
}

void Controller::onReceive(User &user)
{
    std::vector<std::string> payloadHashes;

    for(const auto& payload: user.getReadPayloads())
    {
        size_t payloadHash = reinterpret_cast<size_t>(&payload);
        payloadHashes.push_back(boost::lexical_cast<std::string>(payloadHash));
    }

    view_.emitPayloadReceived(boost::lexical_cast<std::string>(user.getHash()), payloadHashes);
}

void Controller::onClose(User &user)
{
    view_.emitConnectionClosed(boost::lexical_cast<std::string>(user.getHash()));
}

std::string Controller::getReadPayloadDump(const std::string &connectionId, const std::string &payloadId)
{
    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(connectionId));
        User &user = server_.getUsersFactory().find(userHash);

        for(const auto &payload: user.getReadPayloads())
        {
            size_t payloadHash = reinterpret_cast<size_t>(&payload);

            if(payloadId == boost::lexical_cast<std::string>(payloadHash))
            {
                std::stringstream payloadDump; payloadDump << payload;
                return payloadDump.str();
            }
        }
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find user! Id: " << connectionId;
    }

    return "";
}

void Controller::disconnect(const std::string &connectionId)
{
    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(connectionId));
        User &user = server_.getUsersFactory().find(userHash);

        user.getConnection().disconnect();
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find user! Id: " << connectionId;
    }
}

void Controller::send(const std::string &connectionId, const std::string &dump)
{
    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(connectionId));
        User &user = server_.getUsersFactory().find(userHash);

        const core::network::Payload &payload = this->convertDumpToPayload(dump);
        user.getConnection().send(payload);
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find connectionId: " << connectionId;
    }
}

core::network::Payload Controller::convertDumpToPayload(std::string dump) const
{
    dump.erase(std::remove(dump.begin(), dump.end(), ' '), dump.end());

    core::network::Payload payload;
    payload.setSize(dump.length() / 2);

    size_t byte = 0;
    for(size_t i = 0; i < dump.length(); i += 2)
    {
        std::string byteDump(dump, i, 2);

        std::stringstream ss;
        ss << std::hex << byteDump;
        int16_t dec = 0;
        ss >> dec;

        payload[byte++] = dec;
    }

    return payload;
}

}
}
