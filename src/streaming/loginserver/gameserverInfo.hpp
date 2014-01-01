#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

struct GameserverInfo
{
    uint16_t code_;
    std::string name_;
    std::string address_;
    uint16_t port_;
};

typedef std::vector<GameserverInfo> GameserversInfoContainer;

}
}
}
