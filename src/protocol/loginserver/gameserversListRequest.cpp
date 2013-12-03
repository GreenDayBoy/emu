#include <protocol/loginserver/gameserversListRequest.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{

GameserversListRequest::GameserversListRequest(const ReadStream &readStream):
    readStream_(readStream)
{
}

}
}
}
