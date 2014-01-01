#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/dataserver/checkAccountResult.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <string>

namespace eMU
{
namespace streaming
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
