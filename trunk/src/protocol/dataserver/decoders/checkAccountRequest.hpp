#pragma once

#include <protocol/readStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace decoders
{

class CheckAccountRequest
{
public:
    CheckAccountRequest(const ReadStream &readStream);
    CheckAccountRequest(size_t clientHash, const std::string &accountId, const std::string &password);

    size_t getClientHash() const;
    const std::string& getAccountId() const;
    const std::string& getPassword() const;

private:
    ReadStream readStream_;

    size_t clientHash_;
    std::string accountId_;
    std::string password_;
};

}
}
}
}
