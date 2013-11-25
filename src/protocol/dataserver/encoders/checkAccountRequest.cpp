#include <protocol/dataserver/encoders/checkAccountRequest.hpp>
#include <protocol/dataserver/messageIds.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace encoders
{

CheckAccountRequest::CheckAccountRequest(size_t clientHash, const std::string &accountId, const std::string password):
    writeStream_(MessageIds::kCheckAccountRequest)
{
    writeStream_.writeNext<size_t>(clientHash);

    writeStream_.writeNext<uint32_t>(accountId.length());
    writeStream_.writeNext(accountId);

    writeStream_.writeNext<uint32_t>(password.length());
    writeStream_.writeNext(password);
}

const WriteStream& CheckAccountRequest::getWriteStream() const
{
    return writeStream_;
}

}
}
}
}
