#include <streaming/loginserver/gameserverDetailsResponse.hpp>
#include <streaming/loginserver/streamIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

GameserverDetailsResponse::GameserverDetailsResponse(const std::string &ipAddress, uint16_t port):
    writeStream_(streamIds::kGameserverDetailsResponse)
{
    writeStream_.writeNext<uint32_t>(0);
    writeStream_.writeNext<uint32_t>(0);
    writeStream_.writeNext<uint32_t>(0);

    writeStream_.writeNextString(ipAddress);

    for(size_t i = ipAddress.length(); i < 16; ++i)
    {
        writeStream_.writeNext<uint8_t>(0);
    }

    writeStream_.writeNext<uint16_t>(port);
}

const WriteStream& GameserverDetailsResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}
