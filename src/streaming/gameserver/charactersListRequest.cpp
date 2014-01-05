#include <streaming/gameserver/charactersListRequest.hpp>
#include <streaming/gameserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

CharactersListRequest::CharactersListRequest(const ReadStream &readStream):
    readStream_(readStream)
{
}

CharactersListRequest::CharactersListRequest():
    writeStream_(streamIds::kCharactersListRequest)
{
}

const WriteStream& CharactersListRequest::getWriteStream() const
{
    return writeStream_;
}

}
}
}
