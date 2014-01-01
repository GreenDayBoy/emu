#pragma once

#include <stdint.h>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

enum class LoginResult
{
    Succeed = 0,
    InvalidAccountId = 0x4E22,
    InvalidPassword = 0x4E28,
    AccountInUse = 0x4E29
};

}
}
}
