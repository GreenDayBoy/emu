#pragma once

#include <protocol/writeStream.hpp>
#include <protocol/loginserver/loginResult.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace encoders
{

class LoginResponse
{
public:
    LoginResponse(LoginResult result);

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
};

}
}
}
}
