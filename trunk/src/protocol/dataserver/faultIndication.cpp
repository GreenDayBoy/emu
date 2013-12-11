#include <protocol/dataserver/faultIndication.hpp>
#include <protocol/dataserver/messageIds.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{

FaultIndication::FaultIndication(const ReadStream &readStream):
    readStream_(readStream)
{
    clientHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint32_t messageLength = readStream_.readNext<uint32_t>();
    message_ = readStream_.readNextString(messageLength);
}

FaultIndication::FaultIndication(core::network::tcp::NetworkUser::Hash clientHash, const std::string &message):
    writeStream_(MessageIds::kFaultIndication)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(clientHash);
    writeStream_.writeNext<uint32_t>(message.length());
    writeStream_.writeNext(message);
}

const WriteStream& FaultIndication::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash FaultIndication::getClientHash() const
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
