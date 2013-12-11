#pragma once

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{

class CheckAccountResponse
{
public:
    CheckAccountResponse(const ReadStream &readStream);
    CheckAccountResponse(core::network::tcp::NetworkUser::Hash clientHash, CheckAccountResult result);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getClientHash() const;
    CheckAccountResult getResult() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash clientHash_;
    CheckAccountResult result_;
};

}
}
}
