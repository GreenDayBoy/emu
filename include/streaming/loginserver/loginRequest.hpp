#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

class LoginRequest
{
public:
    LoginRequest(const ReadStream &readStream);
    LoginRequest(const std::wstring &accountId, const std::wstring &password);

    const WriteStream& getWriteStream() const;
    const std::string& getAccountId() const;
    const std::string& getPassword() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    std::string accountId_;
    std::string password_;
};

}
}
}
