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

class WorldLoginResponse
{
public:
    WorldLoginResponse(const ReadStream &readStream);
    WorldLoginResponse(uint32_t result);
    uint32_t getResult() const;

    const WriteStream& getWriteStream() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
    uint32_t result_;
};

}
}
}
