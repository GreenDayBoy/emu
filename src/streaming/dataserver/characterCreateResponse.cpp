#include <streaming/dataserver/characterCreateResponse.hpp>
#include <streaming/dataserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

CharacterCreateResponse::CharacterCreateResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();
    result_ = readStream_.readNext<CharacterCreateResult>();
}

CharacterCreateResponse::CharacterCreateResponse(core::network::tcp::NetworkUser::Hash userHash, CharacterCreateResult result):
    writeStream_(streamIds::kCharacterCreateResponse)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);
    writeStream_.writeNext<CharacterCreateResult>(result);
}

const WriteStream& CharacterCreateResponse::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash CharacterCreateResponse::getUserHash() const
{
    return userHash_;
}

CharacterCreateResult CharacterCreateResponse::getResult() const
{
    return result_;
}

}
}
}
