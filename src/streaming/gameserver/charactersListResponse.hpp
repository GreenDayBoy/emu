#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/dataserver/characterListInfo.hpp>

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
    CharactersListResponse(const dataserver::CharacterListInfoContainer &characters);

    const WriteStream& getWriteStream() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
};

}
}
}