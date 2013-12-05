#pragma once

#include <protocol/writeStream.hpp>
#include <protocol/readStream.hpp>
#include <protocol/loginserver/gameserverInfo.hpp>

namespace eMU
{
namespace protocol
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
