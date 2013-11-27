#pragma once

#include <protocol/writeStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace encoders
{

class LoginRequest
{
public:
    LoginRequest(const std::wstring &accountId, const std::wstring &password);

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
};

}
}
}
}
