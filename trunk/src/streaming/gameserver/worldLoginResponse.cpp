#include <streaming/gameserver/worldLoginResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

WorldLoginResponse::WorldLoginResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy
    readStream_.readNext<uint32_t>(); // dummy
    result_ = readStream_.readNext<uint32_t>();
}

WorldLoginResponse::WorldLoginResponse(uint32_t result):
    writeStream_(streamIds::kWorldLoginResponse)
{
    writeStream_.writeNext<uint32_t>(0); // dummy
    writeStream_.writeNext<uint32_t>(0); // dummy
    writeStream_.writeNext<uint32_t>(result); // != 0 means failed
}

uint32_t WorldLoginResponse::getResult() const
{
    return result_;
}

const WriteStream& WorldLoginResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}
