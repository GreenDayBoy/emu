#include <streaming/loginserver/gameserversListRequest.hpp>
#include <streaming/loginserver/messageIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
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
