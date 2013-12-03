#pragma once

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{

class CheckAccountRequest
{
public:
    CheckAccountRequest(const ReadStream &readStream);
    CheckAccountRequest(size_t clientHash, const std::string &accountId, const std::string password);

    const WriteStream& getWriteStream() const;
    size_t getClientHash() const;
    const std::string& getAccountId() const;
    const std::string& getPassword() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    size_t clientHash_;
    std::string accountId_;
    std::string password_;
};

}
}
}
