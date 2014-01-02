#include <streaming/dataserver/faultIndication.hpp>
#include <streaming/dataserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

FaultIndication::FaultIndication(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint32_t messageLength = readStream_.readNext<uint32_t>();
    message_ = readStream_.readNextString(messageLength);
}

FaultIndication::FaultIndication(core::network::tcp::NetworkUser::Hash userHash, const std::string &message):
    writeStream_(streamIds::kFaultIndication)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);
    writeStream_.writeNext<uint32_t>(message.length());
    writeStream_.writeNextString(message);
}

const WriteStream& FaultIndication::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash FaultIndication::getUserHash() const
{
    return userHash_;
}

const std::string& FaultIndication::getMessage() const
{
    return message_;
}

}
}
}
