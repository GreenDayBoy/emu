#pragma once

#include <analyzer/server.hpp>
#include <analyzer/views/main.hpp>

namespace eMU
{
namespace analyzer
{

class Controller
{
public:
    Controller(asio::io_service &ioService);

    void onAccept(User &user);
    void onReceive(User &user);
    void onClose(User &user);
    std::string getReadPayloadDump(const std::string &connectionId, const std::string &payloadId);
    void disconnect(const std::string &connectionId);
    void send(const std::string &connectionId, const std::string &dump);

    Server& getServer();
    views::Main& getMainView();

private:
    core::network::Payload convertDumpToPayload(std::string dump) const;

    Server server_;
    views::Main mainView_;
};

}
}
