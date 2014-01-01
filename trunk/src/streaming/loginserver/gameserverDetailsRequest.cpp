#include <streaming/loginserver/gameserverDetailsRequest.hpp>
#include <streaming/loginserver/messageIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

GameserverDetailsRequest::GameserverDetailsRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy1
    readStream_.readNext<uint32_t>(); // dummy2

    gameserverCode_ = readStream_.readNext<uint16_t>();
}

GameserverDetailsRequest::GameserverDetailsRequest(uint16_t gameserverCode):
    writeStream_(MessageIds::kGameserverDetailsRequest)
{
    writeStream_.writeNext<uint32_t>(0); // dummy1
    writeStream_.writeNext<uint32_t>(0); // dummy2

    writeStream_.writeNext<uint16_t>(gameserverCode);
}

const WriteStream& GameserverDetailsRequest::getWriteStream() const
{
    return writeStream_;
}

uint16_t GameserverDetailsRequest::getGameserverCode() const
{
    return gameserverCode_;
}

}
}
}
