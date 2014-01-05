#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

class CharactersListRequest
{
public:
    CharactersListRequest(const ReadStream &readStream);
    CharactersListRequest();

    const WriteStream& getWriteStream() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
};

}
}
}
