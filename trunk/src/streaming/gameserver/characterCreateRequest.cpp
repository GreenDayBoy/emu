#include <streaming/gameserver/characterCreateRequest.hpp>
#include <streaming/gameserver/streamIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

CharacterCreateRequest::CharacterCreateRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy
    readStream_.readNext<uint32_t>(); // dummy

    uint32_t characterNameLength = readStream_.readNext<uint32_t>();

    characterCreateInfo_.name_ = boost::locale::conv::utf_to_utf<std::string::value_type>(readStream_.readNextWideString(characterNameLength));
    characterCreateInfo_.skin_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.race_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.face_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.faceScars_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.hairType_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.hairColor_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.tatoo_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.skinColor_ = readStream_.readNext<uint8_t>();
}

CharacterCreateRequest::CharacterCreateRequest(const common::CharacterCreateInfo &characterCreateInfo):
    writeStream_(streamIds::kCharacterCreateRequest)
{
    writeStream_.writeNext<uint32_t>(0);
    writeStream_.writeNext<uint32_t>(0);

    std::wstring characterName = boost::locale::conv::utf_to_utf<std::wstring::value_type>(characterCreateInfo.name_);
    writeStream_.writeNext<uint32_t>(characterName.length());
    writeStream_.writeNextWideString(characterName);

    writeStream_.writeNext<uint8_t>(characterCreateInfo.skin_);
    writeStream_.writeNext<uint8_t>(characterCreateInfo.race_);
    writeStream_.writeNext<uint8_t>(characterCreateInfo.face_);
    writeStream_.writeNext<uint8_t>(characterCreateInfo.faceScars_);
    writeStream_.writeNext<uint8_t>(characterCreateInfo.hairType_);
    writeStream_.writeNext<uint8_t>(characterCreateInfo.hairColor_);
    writeStream_.writeNext<uint8_t>(characterCreateInfo.tatoo_);
    writeStream_.writeNext<uint8_t>(characterCreateInfo.skinColor_);

}

const WriteStream& CharacterCreateRequest::getWriteStream() const
{
    return writeStream_;
}

const common::CharacterCreateInfo& CharacterCreateRequest::getCharacterCreateInfo() const
{
    return characterCreateInfo_;
}

}
}
}
