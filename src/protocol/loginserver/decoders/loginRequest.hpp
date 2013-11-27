#pragma once

#include <protocol/readStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace decoders
{

class LoginRequest
{
public:
    LoginRequest(const ReadStream &readStream);

    const std::string& getAccountId() const;
    const std::string& getPassword() const;

private:
    ReadStream readStream_;

    std::string accountId_;
    std::string password_;
};

}
}
}
}
