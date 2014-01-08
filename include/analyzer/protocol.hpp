#pragma once

#include <core/network/tcp/protocol.hpp>
#include <analyzer/controller.hpp>

namespace eMU
{
namespace analyzer
{

class Protocol: public core::network::tcp::Protocol
{
public:
    Protocol(Controller &controller);

    bool attach(core::network::tcp::Connection::Pointer connection);
    void detach(core::network::tcp::Connection::Pointer connection);
    bool dispatch(core::network::tcp::Connection::Pointer connection);

private:
    Controller &controller_;
};

}
}
