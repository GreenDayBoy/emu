#include <streaming/dataserver/charactersListResponse.hpp>
#include <streaming/dataserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

CharactersListResponse::CharactersListResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint8_t charactersCount = readStream_.readNext<uint8_t>();

    for(uint8_t i = 0; i < charactersCount; ++i)
    {
        uint32_t characterNameLength = readStream_.readNext<uint32_t>();

        CharacterListInfo characterListInfo;
        characterListInfo.name_ = readStream_.readNextString(characterNameLength);
        characterListInfo.hairColor_ = readStream_.readNext<uint8_t>();
        characterListInfo.hairType_ = readStream_.readNext<uint8_t>();
        characterListInfo.level_ = readStream_.readNext<uint8_t>();
        characterListInfo.race_ = readStream_.readNext<uint8_t>();
        characterListInfo.tutorialState_ = readStream_.readNext<uint16_t>();
        characters_.push_back(characterListInfo);
    }
}

CharactersListResponse::CharactersListResponse(core::network::tcp::NetworkUser::Hash userHash, const CharacterListInfoContainer &characters):
    writeStream_(streamIds::kCharactersListResponse)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);
    writeStream_.writeNext<uint8_t>(characters.size());

    for(const auto &characterListInfo : characters)
    {
        writeStream_.writeNext<uint32_t>(characterListInfo.name_.length());
        writeStream_.writeNextString(characterListInfo.name_);
        writeStream_.writeNext<uint8_t>(characterListInfo.hairColor_);
        writeStream_.writeNext<uint8_t>(characterListInfo.hairType_);
        writeStream_.writeNext<uint8_t>(characterListInfo.level_);
        writeStream_.writeNext<uint8_t>(characterListInfo.race_);
        writeStream_.writeNext<uint16_t>(characterListInfo.tutorialState_);
    }
}

const WriteStream& CharactersListResponse::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash CharactersListResponse::getUserHash() const
{
    return userHash_;
}

const CharacterListInfoContainer& CharactersListResponse::getCharacters() const
{
    return characters_;
}

}
}
}
