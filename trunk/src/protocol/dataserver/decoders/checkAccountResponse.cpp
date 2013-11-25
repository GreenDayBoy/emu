#include <protocol/dataserver/decoders/checkAccountResponse.hpp>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace decoders
{

CheckAccountResponse::CheckAccountResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    clientHash_ = readStream_.readNext<size_t>();
    result_ = readStream_.readNext<CheckAccountResult>();
}

size_t CheckAccountResponse::getClientHash() const
{
    return clientHash_;
}

CheckAccountResult CheckAccountResponse::getResult() const
{
    return result_;
}

}
}
}
}
