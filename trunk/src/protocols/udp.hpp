#pragma once

#include <core/network/udp/protocol.hpp>
#include <streaming/readStream.hpp>
#include <protocols/contexts/udp.hpp>

namespace eMU
{
namespace protocols
{

class Udp: public core::network::udp::Protocol
{
public:
    Udp(contexts::Udp &context);

    void attach(core::network::udp::Connection& connection);
    void dispatch(core::network::udp::Connection& connection, const boost::asio::ip::udp::endpoint &senderEndpoint);
    void detach(core::network::udp::Connection& connection);

protected:
    virtual void handleReadStream(const streaming::ReadStream &stream, const boost::asio::ip::udp::endpoint &senderEndpoint) = 0;

private:
    contexts::Udp  &context_;
};

}
}
