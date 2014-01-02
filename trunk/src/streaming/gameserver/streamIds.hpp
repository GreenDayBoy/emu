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

const uint16_t kStreamIdBase = 0x2000;
const uint16_t kRegisterUserRequest = kStreamIdBase + 0x0001;
const uint16_t kRegisterUserResponse = kStreamIdBase + 0x0002;

}
}
}
}
