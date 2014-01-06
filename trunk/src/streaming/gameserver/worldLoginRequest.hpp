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

class WorldLoginRequest
{
public:
    WorldLoginRequest(const ReadStream &readStream);
    WorldLoginRequest();

    const WriteStream& getWriteStream() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
};

}
}
}
