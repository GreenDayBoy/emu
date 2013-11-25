#pragma once

#include <protocol/readStream.hpp>
#include <protocol/dataserver/checkAccountResult.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace decoders
{

class CheckAccountResponse
{
public:
    CheckAccountResponse(const ReadStream &readStream);

    size_t getClientHash() const;
    CheckAccountResult getResult() const;

private:
    ReadStream readStream_;

    size_t clientHash_;
    CheckAccountResult result_;
};

}
}
}
}
