#pragma once

#include <analyzer/server.hpp>
#include <analyzer/view.hpp>

#include <map>

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
    void loadReadStream(const std::string &userId, const std::string &streamId);
    void disconnectUser(const std::string &userId);

    Server& getServer();
    View& getView();

private:
    Server server_;
    View view_;
};

}
}
