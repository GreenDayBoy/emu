#include <protocol/loginserver/loginResponse.hpp>
#include <protocol/loginserver/messageIds.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{

LoginResponse::LoginResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    result_ = readStream_.readNext<LoginResult>();
}

LoginResponse::LoginResponse(LoginResult result):
    writeStream_(MessageIds::kLoginResponse)
{
    writeStream_.writeNext<LoginResult>(result);
}

const WriteStream& LoginResponse::getWriteStream() const
{
    return writeStream_;
}

LoginResult LoginResponse::getResult() const
{
    return result_;
}

}
}
}
