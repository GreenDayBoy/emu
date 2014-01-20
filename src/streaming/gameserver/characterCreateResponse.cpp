#include <streaming/gameserver/characterCreateResponse.hpp>
#include <streaming/gameserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

CharacterCreateResponse::CharacterCreateResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy
    readStream_.readNext<uint32_t>(); // dummy
    result_ = readStream_.readNext<CharacterCreateResult>();
}

CharacterCreateResponse::CharacterCreateResponse(CharacterCreateResult result):
    writeStream_(streamIds::kCharacterCreateResponse)
{
    writeStream_.writeNext<uint32_t>(0); // dummy
    writeStream_.writeNext<uint32_t>(0); // dummy
    writeStream_.writeNext<CharacterCreateResult>(result);
}

CharacterCreateResult CharacterCreateResponse::getResult() const
{
    return result_;
}

const WriteStream& CharacterCreateResponse::getWriteStream() const
{
    return writeStream_;
}

}
}
}
