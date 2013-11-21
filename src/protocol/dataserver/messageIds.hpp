#pragma once

#include <stdint.h>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace MessageIds
{

const uint16_t kMessageIdBase = 0x1000;
const uint16_t kCheckAccountRequest = kMessageIdBase + 0x0001;
const uint16_t kCheckAccountResponse = kMessageIdBase + 0x0002;

}
}
}
}
