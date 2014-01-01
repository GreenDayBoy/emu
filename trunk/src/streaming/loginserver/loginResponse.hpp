#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/loginserver/loginResult.hpp>

#include <string>

namespace eMU
{
namespace streaming
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
