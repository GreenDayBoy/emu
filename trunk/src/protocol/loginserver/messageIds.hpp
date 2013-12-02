#pragma once

#include <stdint.h>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace MessageIds
{

const uint16_t kMessageIdBase = 0x04C6;
const uint16_t kLoginRequest = kMessageIdBase + 0x0001;
const uint16_t kLoginResponse = kMessageIdBase + 0x0002;
const uint16_t kGameServersListRequest = kMessageIdBase + 0x0003;
const uint16_t kGameServersListResponse = kMessageIdBase + 0x0004;

}
}
}
}
