#include <protocol/dataserver/checkAccountResponse.hpp>
#include <protocol/dataserver/messageIds.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{

CheckAccountResponse::CheckAccountResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    clientHash_ = readStream_.readNext<size_t>();
    result_ = readStream_.readNext<CheckAccountResult>();
}

CheckAccountResponse::CheckAccountResponse(size_t clientHash, CheckAccountResult result):
    writeStream_(MessageIds::kCheckAccountResponse)
{
    writeStream_.writeNext<size_t>(clientHash);
    writeStream_.writeNext<CheckAccountResult>(result);
}

const WriteStream& CheckAccountResponse::getWriteStream() const
{
    return writeStream_;
}

size_t CheckAccountResponse::getClientHash() const
{
    return clientHash_;
}

CheckAccountResult CheckAccountResponse::getResult() const
{
    return result_;
}

}
}
}
