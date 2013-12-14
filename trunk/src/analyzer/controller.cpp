#include <analyzer/controller.hpp>

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
    view_.displayUser(user);
}

void Controller::onReceive(User &user)
{
    view_.displayUserReadStreams(user);
}

void Controller::onClose(User &user)
{
    view_.removeUserFromDisplay(user);
}

}
}
