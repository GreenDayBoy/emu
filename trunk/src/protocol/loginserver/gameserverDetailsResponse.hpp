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

class GameserverDetailsResponse
{
public:
    GameserverDetailsResponse();

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
    ReadStream readStream_;
};

}
}
}
