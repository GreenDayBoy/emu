#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/common/characterInfo.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

class CharactersListResponse
{
public:
    CharactersListResponse(const ReadStream &readStream);
    CharactersListResponse(core::network::tcp::NetworkUser::Hash userHash, const common::CharacterInfoContainer &characters);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getUserHash() const;
    const common::CharacterInfoContainer& getCharacters() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash userHash_;
    common::CharacterInfoContainer characters_;
};

}
}
}
