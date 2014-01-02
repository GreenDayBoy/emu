#include <streaming/loginserver/gameserverDetailsResponse.hpp>
#include <streaming/loginserver/streamIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

GameserverDetailsResponse::GameserverDetailsResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy1
    readStream_.readNext<uint32_t>(); // dummy2
    readStream_.readNext<uint32_t>(); // dummy3

    ipAddress_ = readStream_.readNextString(16);
    port_ = readStream_.readNext<uint16_t>();
}

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

std::string GameserverDetailsResponse::getIpAddress() const
{
    return ipAddress_;
}

uint16_t GameserverDetailsResponse::getPort() const
{
    return port_;
}

}
}
}
