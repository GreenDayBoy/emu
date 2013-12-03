#pragma once

#include <protocol/readStream.hpp>

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

private:
    ReadStream readStream_;
};

}
}
}
