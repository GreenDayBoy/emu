#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/common/characterInfo.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

class CharactersListResponse
{
public:
    CharactersListResponse(const ReadStream &readStream);
    CharactersListResponse(const common::CharacterInfoContainer &characters);

    const WriteStream& getWriteStream() const;
    const common::CharacterInfoContainer& getCharacters() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
    common::CharacterInfoContainer characters_;
};

}
}
}
