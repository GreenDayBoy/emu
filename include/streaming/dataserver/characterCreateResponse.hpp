#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/dataserver/characterCreateResult.hpp>
#include <core/network/tcp/networkUser.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

class CharacterCreateResponse
{
public:
    CharacterCreateResponse(const ReadStream &readStream);
    CharacterCreateResponse(core::network::tcp::NetworkUser::Hash userHash, CharacterCreateResult result);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getUserHash() const;
    CharacterCreateResult getResult() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash userHash_;
    CharacterCreateResult result_;
};

}
}
}
