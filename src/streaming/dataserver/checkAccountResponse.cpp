#include <streaming/dataserver/checkAccountResponse.hpp>
#include <streaming/dataserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

CheckAccountResponse::CheckAccountResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();
    result_ = readStream_.readNext<CheckAccountResult>();
}

CheckAccountResponse::CheckAccountResponse(core::network::tcp::NetworkUser::Hash userHash, CheckAccountResult result):
    writeStream_(streamIds::kCheckAccountResponse)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);
    writeStream_.writeNext<CheckAccountResult>(result);
}

const WriteStream& CheckAccountResponse::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash CheckAccountResponse::getUserHash() const
{
    return userHash_;
}

CheckAccountResult CheckAccountResponse::getResult() const
{
    return result_;
}

}
}
}
