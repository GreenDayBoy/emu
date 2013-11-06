#pragma once

#include <core/network/payload.hpp>
#include <core/common/exception.hpp>

#include <stdint.h>

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

    class EmptyPayloadException: public common::Exception {};
    class InvalidMessageSizeException: public common::Exception {};
    class InvalidMessageHeaderException: public common::Exception {};

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
