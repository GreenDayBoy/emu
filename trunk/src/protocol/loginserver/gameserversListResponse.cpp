#include <protocol/loginserver/gameserversListResponse.hpp>
#include <protocol/loginserver/messageIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{

GameserversListResponse::GameserversListResponse(const GameserversInfoContainer &servers):
    writeStream_(MessageIds::kGameserversListResponse)
{
    writeStream_.writeNext<uint32_t>(0); // dummy1
    writeStream_.writeNext<uint32_t>(0); // dummy1
    writeStream_.writeNext<uint16_t>(servers.size());

    for(const auto &info : servers)
    {
        writeStream_.writeNext<uint32_t>(info.code_);
        writeStream_.writeNext<uint32_t>(10); // dummy1
        writeStream_.writeNext<uint32_t>(20); // dummy2
        writeStream_.writeNext<uint32_t>(30); // dummy3
        writeStream_.writeNext<uint8_t>(40); // dummy4
        writeStream_.writeNext<uint32_t>(info.name_.length());

        std::wstring name = boost::locale::conv::utf_to_utf<std::wstring::value_type>(info.name_);
        writeStream_.writeNext<>(name);
    }
}

const WriteStream& GameserversListResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}
