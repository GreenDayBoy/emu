#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/common/characterViewInfo.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

class CharacterCreateRequest
{
public:
    CharacterCreateRequest(const ReadStream &readStream);
    CharacterCreateRequest(const common::CharacterViewInfo &characterCreateInfo);

    const WriteStream& getWriteStream() const;
    const common::CharacterViewInfo& getCharacterCreateInfo() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
    common::CharacterViewInfo characterCreateInfo_;
};

}
}
}
