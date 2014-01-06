#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/dataserver/characterListInfo.hpp>
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
    CharactersListResponse(core::network::tcp::NetworkUser::Hash userHash, const CharacterListInfoContainer &characters);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getUserHash() const;
    const CharacterListInfoContainer& getCharacters() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash userHash_;
    CharacterListInfoContainer characters_;
};

}
}
}
