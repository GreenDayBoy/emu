#include <protocol/loginserver/gameserverDetailsResponse.hpp>
#include <protocol/loginserver/messageIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{

GameserverDetailsResponse::GameserverDetailsResponse():
    writeStream_(MessageIds::kGameserverDetailsResponse)
{
    writeStream_.writeNext<uint32_t>(0);
    std::string ip = "10.0.0.3";
    writeStream_.writeNext(ip);
    writeStream_.writeNext<uint32_t>(0);
    writeStream_.writeNext<uint32_t>(0);

    writeStream_.writeNext<uint16_t>(55960);
}

const WriteStream& GameserverDetailsResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}
