#pragma once

#include <stdint.h>

const uint16_t kDataServerProtocolBase = 0xF00;

namespace interface
{
namespace DataServerMessageIds
{

enum Ids
{
    CheckAccountRequest = kDataServerProtocolBase + 1,
};

}
}
