#pragma once

#include <stdint.h>

namespace eMU
{
namespace interface
{
namespace dataserver
{
namespace ids
{

const uint16_t kDataServerMessageIdBase = 0xF00;

enum Ids
{
    CheckAccountRequest = kDataServerMessageIdBase + 1,
};

}
}
}
}
