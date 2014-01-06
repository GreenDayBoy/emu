#pragma once

#include <stdint.h>

namespace eMU
{
namespace streaming
{
namespace gameserver
{
namespace streamIds
{

const uint16_t kStreamIdBase = 0x0136;
const uint16_t kRegisterUserRequest = kStreamIdBase + 0x0001;
const uint16_t kRegisterUserResponse = kStreamIdBase + 0x0002;
const uint16_t kWorldLoginRequest = kStreamIdBase + 0x0003;
const uint16_t kWorldLoginResponse = kStreamIdBase + 0x0004;
const uint16_t kCharactersListRequest = kStreamIdBase + 0x0005;
const uint16_t kCharactersListResponse = kStreamIdBase + 0x0006;

}
}
}
}
