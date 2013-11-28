#include <protocol/loginserver/decoders/loginResponse.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace decoders
{

LoginResponse::LoginResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    result_ = readStream_.readNext<LoginResult>();
}

LoginResult LoginResponse::getResult() const
{
    return result_;
}

}
}
}
}
