#include <core/protocol/messagesExtractor.hpp>
#include <core/protocol/helpers.hpp>
#include <core/protocol/exceptions.hpp>

namespace eMU
{
namespace core
{
namespace protocol
{

MessagesExtractor::MessagesExtractor(const network::Payload &payload):
    payload_(payload) {}

void MessagesExtractor::extract()
{
    if(payload_.empty())
    {
        throw exceptions::EmptyPayloadException();
    }

    size_t totalSize = 0;

    while(totalSize < payload_.size())
    {
        network::Payload frame(payload_.begin() + totalSize, payload_.end());

        if(!hasValidHeader(frame))
        {
            throw exceptions::InvalidMessageHeaderException();
        }

        size_t size = getSize(frame);

        if(frame.size() < size)
        {
            throw exceptions::InvalidMessageSizeException();
        }

        payloads_.push_back(network::Payload(frame.begin(), frame.begin() + size));
        totalSize += size;
    }
}

const MessagesExtractor::PayloadsContainer& MessagesExtractor::payloads() const
{
    return payloads_;
}

}
}
}
