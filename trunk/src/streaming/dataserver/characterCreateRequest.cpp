#include <streaming/dataserver/characterCreateRequest.hpp>
#include <streaming/dataserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

CharacterCreateRequest::CharacterCreateRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint32_t accountIdLength = readStream_.readNext<uint32_t>();
    accountId_ = readStream_.readNextString(accountIdLength);

    uint32_t characterNameLength = readStream_.readNext<uint32_t>();
    characterCreateInfo_.name_ = readStream_.readNextString(characterNameLength);
    characterCreateInfo_.skin_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.race_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.face_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.faceScars_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.hairType_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.hairColor_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.tatoo_ = readStream_.readNext<uint8_t>();
    characterCreateInfo_.skinColor_ = readStream_.readNext<uint8_t>();
}

CharacterCreateRequest::CharacterCreateRequest(core::network::tcp::NetworkUser::Hash userHash,
                                               const std::string &accountId,
                                               const common::CharacterCreateInfo &characterCreateInfo):
    writeStream_(streamIds::kCharacterCreateRequest)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);

    writeStream_.writeNext<uint32_t>(accountId.length());
    writeStream_.writeNextString(accountId);

    writeStream_.writeNext<uint32_t>(characterCreateInfo.name_.length());
    writeStream_.writeNextString(characterCreateInfo.name_);
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

core::network::tcp::NetworkUser::Hash CharacterCreateRequest::getUserHash() const
{
    return userHash_;
}

const std::string& CharacterCreateRequest::getAccountId() const
{
    return accountId_;
}

const common::CharacterCreateInfo& CharacterCreateRequest::getCharacterCreateInfo() const
{
    return characterCreateInfo_;
}

}
}
}
