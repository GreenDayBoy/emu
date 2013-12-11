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
const uint16_t kGameserversListRequest = kMessageIdBase + 0x0003;
const uint16_t kGameserversListResponse = kMessageIdBase + 0x0004;
const uint16_t kGameserverDetailsRequest = kMessageIdBase + 0x0005;
const uint16_t kGameserverDetailsResponse = kMessageIdBase + 0x0006;

}
}
}
}
