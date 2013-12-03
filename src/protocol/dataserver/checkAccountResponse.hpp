#pragma once

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{

class CheckAccountResponse
{
public:
    CheckAccountResponse(const ReadStream &readStream);
    CheckAccountResponse(size_t clientHash, CheckAccountResult result);

    const WriteStream& getWriteStream() const;
    size_t getClientHash() const;
    CheckAccountResult getResult() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    size_t clientHash_;
    CheckAccountResult result_;
};

}
}
}
