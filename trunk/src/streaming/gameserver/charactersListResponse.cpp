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
    readStream_.readNext<uint32_t>();
    readStream_.readNext<uint32_t>();
    readStream_.readNext<uint32_t>();

    uint16_t charactersCount = readStream_.readNext<uint16_t>();

    for(uint16_t i = 0; i < charactersCount; ++i)
    {
        readStream_.readNext<uint32_t>();
        uint32_t characterNameLength = readStream_.readNext<uint32_t>();

        common::CharacterListInfo characterListInfo;
        characterListInfo.name_ = boost::locale::conv::utf_to_utf<std::string::value_type>(readStream_.readNextWideString(characterNameLength));
        characterListInfo.level_ = readStream_.readNext<uint8_t>();
        readStream_.readNext<uint8_t>();
        readStream_.readNext<uint8_t>();
        characterListInfo.race_ = readStream_.readNext<uint8_t>();
        readStream_.readNext<uint16_t>();
        characterListInfo.hairType_ = readStream_.readNext<uint8_t>();
        characterListInfo.hairColor_ = readStream_.readNext<uint8_t>();
        readStream_.readNext<uint32_t>();
        readStream_.readNext<uint32_t>();
        readStream_.readNext<uint16_t>();
        characterListInfo.tutorialState_ = readStream_.readNext<uint16_t>();

        characters_.push_back(characterListInfo);
    }
}

CharactersListResponse::CharactersListResponse(const common::CharacterInfoContainer &characters):
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
        writeStream_.writeNext<uint16_t>(0); // dummy
        writeStream_.writeNext<uint8_t>(characterInfo.hairType_);
        writeStream_.writeNext<uint8_t>(characterInfo.hairColor_);
        writeStream_.writeNext<uint32_t>(0); // dummy
        writeStream_.writeNext<uint32_t>(0); // dummy
        writeStream_.writeNext<uint16_t>(0); // dummy
        writeStream_.writeNext<uint16_t>(characterInfo.tutorialState_); // tutorial state
    }
}

const WriteStream& CharactersListResponse::getWriteStream() const
{
    return writeStream_;
}

const common::CharacterInfoContainer& CharactersListResponse::getCharacters() const
{
    return characters_;
}

}
}
}
