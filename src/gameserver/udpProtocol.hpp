#pragma once

#include <protocols/udp.hpp>
#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
{

class UdpProtocol: public protocols::Udp
{
public:
    UdpProtocol(Context &context);

private:
    void handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint);

    Context &context_;
};

}
}
