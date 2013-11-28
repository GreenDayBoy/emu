#pragma once

#include <protocol/readStream.hpp>
#include <protocol/loginserver/loginResult.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace decoders
{

class LoginResponse
{
public:
    LoginResponse(const ReadStream &readStream);

    LoginResult getResult() const;

private:
    ReadStream readStream_;

    LoginResult result_;
};

}
}
}
}
