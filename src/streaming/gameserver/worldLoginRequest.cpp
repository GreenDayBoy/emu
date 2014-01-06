#include <streaming/gameserver/worldLoginRequest.hpp>
#include <streaming/gameserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

WorldLoginRequest::WorldLoginRequest(const ReadStream &readStream):
    readStream_(readStream)
{
}

WorldLoginRequest::WorldLoginRequest():
    writeStream_(streamIds::kWorldLoginRequest)
{
}

const WriteStream& WorldLoginRequest::getWriteStream() const
{
    return writeStream_;
}

}
}
}
