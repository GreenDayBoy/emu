#include <streaming/loginserver/gameserversListResponse.hpp>
#include <streaming/loginserver/streamIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

GameserversListResponse::GameserversListResponse(const GameserversInfoContainer &servers):
    writeStream_(streamIds::kGameserversListResponse)
{
    writeStream_.writeNext<uint32_t>(0); // dummy1
    writeStream_.writeNext<uint32_t>(0); // dummy2
    writeStream_.writeNext<uint32_t>(0); // dummy3
    writeStream_.writeNext<uint16_t>(servers.size());

    for(const auto &info : servers)
    {
        writeStream_.writeNext<uint32_t>(info.code_);
        writeStream_.writeNext<uint32_t>(0); // dummy1
        writeStream_.writeNext<uint32_t>(0); // dummy2
        writeStream_.writeNext<uint32_t>(0); // dummy3
        writeStream_.writeNext<uint8_t>(0); // dummy4
        writeStream_.writeNext<uint32_t>(info.name_.length());

        std::wstring name = boost::locale::conv::utf_to_utf<std::wstring::value_type>(info.name_);
        writeStream_.writeNextWideString(name);

        writeStream_.writeNext<uint32_t>(0); // dummy3
        writeStream_.writeNext<uint32_t>(0); // dummy3
    }
}

GameserversListResponse::GameserversListResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy1
    readStream_.readNext<uint32_t>(); // dummy2
    readStream_.readNext<uint32_t>(); // dummy3

    size_t numberOfServers = readStream_.readNext<uint16_t>();

    for(size_t i = 0; i < numberOfServers; ++i)
    {
        GameserverInfo info = {0};
        info.code_ = readStream_.readNext<uint32_t>();

        readStream_.readNext<uint32_t>(); // dummy1
        readStream_.readNext<uint32_t>(); // dummy2
        readStream_.readNext<uint32_t>(); // dummy3
        readStream_.readNext<uint8_t>(); // dummy4

        size_t nameLength = readStream_.readNext<uint32_t>();
        info.name_ = boost::locale::conv::utf_to_utf<std::string::value_type>(readStream_.readNextWideString(nameLength));

        readStream_.readNext<uint32_t>(); // dummy3
        readStream_.readNext<uint32_t>(); // dummy3

        servers_.push_back(info);
    }
}

const WriteStream& GameserversListResponse::getWriteStream() const
{
    return writeStream_;
}

const GameserversInfoContainer& GameserversListResponse::getServers() const
{
    return servers_;
}

}
}
}
