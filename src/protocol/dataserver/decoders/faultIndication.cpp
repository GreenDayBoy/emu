#include <protocol/dataserver/decoders/faultIndication.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace decoders
{

FaultIndication::FaultIndication(const ReadStream &readStream):
    readStream_(readStream)
{
    clientHash_ = readStream_.readNext<size_t>();

    uint32_t messageLength = readStream_.readNext<uint32_t>();
    message_ = readStream_.readNextString(messageLength);
}

size_t FaultIndication::getClientHash() const
{
    return clientHash_;
}

const std::string& FaultIndication::getMessage() const
{
    return message_;
}

}
}
}
}
