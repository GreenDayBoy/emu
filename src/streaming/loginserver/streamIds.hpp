#pragma once

#include <stdint.h>

namespace eMU
{
namespace streaming
{
namespace loginserver
{
namespace streamIds
{

const uint16_t kStreamIdBase = 0x04C6;
const uint16_t kLoginRequest = kStreamIdBase + 0x0001;
const uint16_t kLoginResponse = kStreamIdBase + 0x0002;
const uint16_t kGameserversListRequest = kStreamIdBase + 0x0003;
const uint16_t kGameserversListResponse = kStreamIdBase + 0x0004;
const uint16_t kGameserverDetailsRequest = kStreamIdBase + 0x0005;
const uint16_t kGameserverDetailsResponse = kStreamIdBase + 0x0006;

}
}
}
}
