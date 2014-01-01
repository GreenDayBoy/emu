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

class GameserverDetailsRequest
{
public:
    GameserverDetailsRequest(const ReadStream &readStream);
    GameserverDetailsRequest(uint16_t gameserverCode);

    const WriteStream& getWriteStream() const;

    uint16_t getGameserverCode() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    uint16_t gameserverCode_;
};

}
}
}
