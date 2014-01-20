#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/gameserver/characterCreateResult.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

class CharacterCreateResponse
{
public:
    CharacterCreateResponse(const ReadStream &readStream);
    CharacterCreateResponse(CharacterCreateResult result);

    CharacterCreateResult getResult() const;
    const WriteStream& getWriteStream() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
    CharacterCreateResult result_;
};

}
}
}
