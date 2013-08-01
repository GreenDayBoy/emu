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
    typedef std::vector<network::Payload> MessagesContainer;

    MessagesExtractor(const network::Payload &payload);

    void extract();
    const MessagesContainer& messages() const;

private:
    MessagesExtractor();

    const network::Payload &payload_;
    MessagesContainer messages_;
};

}
}
}
