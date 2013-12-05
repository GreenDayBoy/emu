#pragma once

namespace eMU
{
namespace protocol
{
namespace dataserver
{

enum class CheckAccountResult
{
    Succeed = 0,
    InvalidAccountId,
    InvalidPassword,
    AccountInUse
};


}
}
}
