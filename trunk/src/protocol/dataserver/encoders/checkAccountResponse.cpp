#include <protocol/dataserver/encoders/checkAccountResponse.hpp>
#include <protocol/dataserver/messageIds.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace encoders
{

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

}
}
}
}
