#include <protocol/loginserver/encoders/loginResponse.hpp>
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

LoginResponse::LoginResponse(LoginResult result):
    writeStream_(MessageIds::kLoginResponse)
{
    writeStream_.writeNext<LoginResult>(result);
}

const WriteStream& LoginResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}
}
