#pragma once

namespace eMU
{
namespace streaming
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
