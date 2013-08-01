#pragma once

#include <stdint.h>
#include <core/network/buffer.hpp>

namespace eMU
{
namespace core
{
namespace protocol
{

class MessagesExtractor
{
public:
    typedef std::vector<network::Payload> PayloadsContainer;

    MessagesExtractor(const network::Payload &payload);

    void extract();
    const PayloadsContainer& payloads() const;

private:
    MessagesExtractor();

    const network::Payload &payload_;
    PayloadsContainer payloads_;
};

}
}
}
