#include <protocol/dataserver/encoders/faultIndication.hpp>
#include <protocol/dataserver/messageIds.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace encoders
{

FaultIndication::FaultIndication(size_t clientHash, const std::string &message):
    writeStream_(MessageIds::kFaultIndication)
{
    writeStream_.writeNext<size_t>(clientHash);
    writeStream_.writeNext<uint32_t>(message.length());
    writeStream_.writeNext(message);
}

const WriteStream& FaultIndication::getWriteStream() const
{
    return writeStream_;
}

}
}
}
}
