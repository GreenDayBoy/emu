#pragma once

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>
#include <protocol/loginserver/loginResult.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace loginserver
{

class LoginResponse
{
public:
    LoginResponse(const ReadStream &readStream);
    LoginResponse(LoginResult result);

    const WriteStream& getWriteStream() const;
    LoginResult getResult() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    LoginResult result_;
};

}
}
}
