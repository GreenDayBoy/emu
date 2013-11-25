#pragma once

#include <protocol/writeStream.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace encoders
{

class CheckAccountRequest
{
public:
    CheckAccountRequest(size_t clientHash, const std::string &accountId, const std::string password);

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
};

}
}
}
}
