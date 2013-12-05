#include <protocol/loginserver/gameserversListRequest.hpp>
#include <protocol/loginserver/messageIds.hpp>

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

GameserversListRequest::GameserversListRequest():
    writeStream_(MessageIds::kGameserversListRequest)
{

}

const WriteStream& GameserversListRequest::getWriteStream() const
{
    return writeStream_;
}

}
}
}
