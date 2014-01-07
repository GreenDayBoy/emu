#include <streaming/gameserver/charactersListResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

CharactersListResponse::CharactersListResponse(const ReadStream &readStream):
    readStream_(readStream)
{
}

CharactersListResponse::CharactersListResponse(const dataserver::CharacterListInfoContainer &characters):
    writeStream_(streamIds::kCharactersListResponse)
{
    writeStream_.writeNext<uint32_t>(0); // dummy
    writeStream_.writeNext<uint32_t>(0); // dummy
    writeStream_.writeNext<uint32_t>(0); // dummy
    writeStream_.writeNext<uint16_t>(characters.size());

    for(const auto &characterInfo : characters)
    {
        writeStream_.writeNext<uint32_t>(0); // dummy
        std::wstring characterName = boost::locale::conv::utf_to_utf<std::wstring::value_type>(characterInfo.name_);
        writeStream_.writeNext<uint32_t>(characterName.length());
        writeStream_.writeNextWideString(characterName);
        writeStream_.writeNext<uint8_t>(characterInfo.level_);
        writeStream_.writeNext<uint8_t>(0); // dummy
        writeStream_.writeNext<uint8_t>(1); // dummy
        writeStream_.writeNext<uint8_t>(characterInfo.race_);
        writeStream_.writeNext<uint8_t>(0); // dummy
        writeStream_.writeNext<uint8_t>(0); // dummy
        writeStream_.writeNext<uint8_t>(characterInfo.hairType_);
        writeStream_.writeNext<uint8_t>(characterInfo.hairColor_);
        writeStream_.writeNext<uint32_t>(0); // dummy
        writeStream_.writeNext<uint16_t>(0); // dummy
        writeStream_.writeNext<uint32_t>(0); // dummy
        writeStream_.writeNext<uint16_t>(characterInfo.tutorialState_); // tutorial state
    }
}

const WriteStream& CharactersListResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}
