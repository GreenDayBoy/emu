#pragma once

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
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
