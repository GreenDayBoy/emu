#pragma once

#include <stdint.h>

namespace eMU
{
namespace streaming
{
namespace dataserver
{
namespace streamIds
{

const uint16_t kStreamIdBase = 0x1000;
const uint16_t kFaultIndication = kStreamIdBase + 0x0001;
const uint16_t kCheckAccountRequest = kStreamIdBase + 0x0002;
const uint16_t kCheckAccountResponse = kStreamIdBase + 0x0003;
const uint16_t kCharactersListRequest = kStreamIdBase + 0x0004;
const uint16_t kCharactersListResponse = kStreamIdBase + 0x0005;

}
}
}
}
