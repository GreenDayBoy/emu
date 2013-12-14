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

    Server& getServer();
    View& getView();

private:
    Server server_;
    View view_;
};

}
}
