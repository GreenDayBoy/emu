#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/dataserver/messageIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

CheckAccountResponse::CheckAccountResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    clientHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();
    result_ = readStream_.readNext<CheckAccountResult>();
}

CheckAccountResponse::CheckAccountResponse(core::network::tcp::NetworkUser::Hash clientHash, CheckAccountResult result):
    writeStream_(MessageIds::kCheckAccountResponse)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(clientHash);
    writeStream_.writeNext<CheckAccountResult>(result);
}

const WriteStream& CheckAccountResponse::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash CheckAccountResponse::getClientHash() const
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
