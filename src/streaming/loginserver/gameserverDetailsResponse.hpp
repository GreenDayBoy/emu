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
    GameserverDetailsResponse(const ReadStream &readStream);
    GameserverDetailsResponse(const std::string &ipAddress, uint16_t port);

    const WriteStream& getWriteStream() const;

    std::string getIpAddress() const;
    uint16_t getPort() const;

private:
    WriteStream writeStream_;
    ReadStream readStream_;

    std::string ipAddress_;
    uint16_t port_;
};

}
}
}
