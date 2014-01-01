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

class GameserverDetailsResponse
{
public:
    GameserverDetailsResponse(const std::string &ipAddress, uint16_t port);

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
    ReadStream readStream_;
};

}
}
}
