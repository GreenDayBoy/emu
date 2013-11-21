#pragma once

#include <protocol/writeStream.hpp>
#include <protocol/dataserver/CheckAccountResult.hpp>

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
    CheckAccountResponse(size_t clientHash, CheckAccountResult result);

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
};

}
}
}
}
