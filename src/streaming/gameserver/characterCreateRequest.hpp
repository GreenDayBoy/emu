#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <streaming/common/characterCreateInfo.hpp>

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
    CharacterCreateRequest(const common::CharacterCreateInfo &characterCreateInfo);

    const WriteStream& getWriteStream() const;
    const common::CharacterCreateInfo& getCharacterCreateInfo() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
    common::CharacterCreateInfo characterCreateInfo_;
};

}
}
}
