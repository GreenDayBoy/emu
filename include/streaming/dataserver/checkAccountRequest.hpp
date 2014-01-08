#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

class CheckAccountRequest
{
public:
    CheckAccountRequest(const ReadStream &readStream);
    CheckAccountRequest(core::network::tcp::NetworkUser::Hash userHash, const std::string &accountId, const std::string password);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getUserHash() const;
    const std::string& getAccountId() const;
    const std::string& getPassword() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash userHash_;
    std::string accountId_;
    std::string password_;
};

}
}
}
