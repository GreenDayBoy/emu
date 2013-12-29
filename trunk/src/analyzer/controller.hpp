#pragma once

#include <analyzer/user.hpp>
#include <analyzer/views/main.hpp>
#include <core/common/factory.hpp>

namespace eMU
{
namespace analyzer
{

class Controller
{
public:
    Controller(size_t maxNumberOfUsers);

    void onAccept(User &user);
    void onReceive(User &user);
    void onClose(User &user);
    std::string getReadPayloadDump(const std::string &connectionId, const std::string &payloadId);
    void disconnect(const std::string &connectionId);
    void send(const std::string &connectionId, const std::string &dump);

    core::common::Factory<User>& getUsersFactory();
    size_t getMaxNumberOfUsers();

    views::Main& getMainView();

private:
    core::network::Payload convertDumpToPayload(std::string dump) const;

    core::common::Factory<User> usersFactory_;
    size_t maxNumberOfUsers_;
    views::Main mainView_;
};

}
}
