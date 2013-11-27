#include <protocol/loginserver/encoders/loginRequest.hpp>
#include <protocol/loginserver/messageIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace encoders
{

LoginRequest::LoginRequest(const std::wstring &accountId, const std::wstring &password):
    writeStream_(MessageIds::kLoginRequest)
{
    writeStream_.writeNext<uint32_t>(accountId.length());
    writeStream_.writeNext(accountId);

    writeStream_.writeNext<uint32_t>(password.length());
    writeStream_.writeNext(password);
}

const WriteStream& LoginRequest::getWriteStream() const
{
    return writeStream_;
}

}
}
}
}
