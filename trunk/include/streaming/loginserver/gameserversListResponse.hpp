#pragma once

#include <streaming/writeStream.hpp>
#include <streaming/readStream.hpp>
#include <streaming/loginserver/gameserverInfo.hpp>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

class GameserversListResponse
{
public:
    GameserversListResponse(const GameserversInfoContainer &servers);
    GameserversListResponse(const ReadStream &readStream);

    const WriteStream& getWriteStream() const;

    const GameserversInfoContainer& getServers() const;

private:
    WriteStream writeStream_;
    ReadStream readStream_;

    GameserversInfoContainer servers_;
};

}
}
}
