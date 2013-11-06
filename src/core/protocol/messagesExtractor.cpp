#include <core/protocol/messagesExtractor.hpp>
#include <core/protocol/helpers.hpp>

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
//    if(payload_.empty())
//    {
//        throw EmptyPayloadException();
//    }

//    size_t totalSize = 0;

//    while(totalSize < payload_.size())
//    {
//        network::Payload message(payload_.begin() + totalSize, payload_.end());

//        if(!hasValidHeader(message))
//        {
//            throw InvalidMessageHeaderException();
//        }

//        size_t messageSize = getMessageSize(message);

//        if(message.size() < messageSize || messageSize == 0)
//        {
//            throw InvalidMessageSizeException();
//        }

//        messages_.push_back(network::Payload(message.begin(), message.begin() + messageSize));
//        totalSize += messageSize;
//    }
}

const MessagesExtractor::MessagesContainer &MessagesExtractor::messages() const
{
    return messages_;
}

}
}
}
