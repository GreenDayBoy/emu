#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

class RegisterUserRequest
{
public:
    RegisterUserRequest(const ReadStream &readStream);
    RegisterUserRequest(core::network::tcp::NetworkUser::Hash userHash, const std::string &accountId);

    const WriteStream& getWriteStream() const;

    core::network::tcp::NetworkUser::Hash getUserHash() const;
    std::string getAccountId() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash userHash_;
    std::string accountId_;
};

}
}
}
