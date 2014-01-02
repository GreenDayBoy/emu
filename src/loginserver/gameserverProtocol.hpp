#pragma once

#include <protocols/udp.hpp>
#include <loginserver/context.hpp>

namespace eMU
{
namespace loginserver
{

class GameserverProtocol: public protocols::Udp
{
public:
    GameserverProtocol(Context &context);

private:
    void handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint);

    Context &context_;
};

}
}
