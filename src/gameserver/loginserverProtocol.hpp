#pragma once

#include <protocols/udp.hpp>
#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
{

class LoginserverProtocol: public protocols::Udp
{
public:
    LoginserverProtocol(Context &context);

private:
    void handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint);

    Context &context_;
};

}
}
