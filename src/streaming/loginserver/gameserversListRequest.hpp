#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

class GameserversListRequest
{
public:
    GameserversListRequest(const ReadStream &readStream);
    GameserversListRequest();

    const WriteStream& getWriteStream() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;
};

}
}
}
