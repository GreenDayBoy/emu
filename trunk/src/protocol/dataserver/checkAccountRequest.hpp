#pragma once

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>
#include <core/network/tcp/networkUser.hpp>

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
    CheckAccountRequest(core::network::tcp::NetworkUser::Hash clientHash, const std::string &accountId, const std::string password);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getClientHash() const;
    const std::string& getAccountId() const;
    const std::string& getPassword() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash clientHash_;
    std::string accountId_;
    std::string password_;
};

}
}
}
