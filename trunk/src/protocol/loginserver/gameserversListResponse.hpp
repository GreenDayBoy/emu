#pragma once

#include <protocol/writeStream.hpp>
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

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
};

}
}
}
