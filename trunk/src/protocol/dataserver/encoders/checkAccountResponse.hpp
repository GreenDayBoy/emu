#pragma once

#include <protocol/writeStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace encoders
{

class CheckAccountResponse
{
public:
    enum class CheckAccountResult
    {
        Succeed = 0,
        Failed,
        AcoountInUse
    };

    CheckAccountResponse(size_t clientHash, CheckAccountResult result);

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
};

}
}
}
}
