#include <streaming/dataserver/charactersListRequest.hpp>
#include <streaming/dataserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

CharactersListRequest::CharactersListRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    userHash_ = readStream_.readNext<core::network::tcp::NetworkUser::Hash>();

    uint32_t accountIdLength = readStream_.readNext<uint32_t>();
    accountId_ = readStream_.readNextString(accountIdLength);
}

CharactersListRequest::CharactersListRequest(core::network::tcp::NetworkUser::Hash userHash, const std::string &accountId):
    writeStream_(streamIds::kCharactersListRequest)
{
    writeStream_.writeNext<core::network::tcp::NetworkUser::Hash>(userHash);

    writeStream_.writeNext<uint32_t>(accountId.length());
    writeStream_.writeNextString(accountId);
}

const WriteStream& CharactersListRequest::getWriteStream() const
{
    return writeStream_;
}

core::network::tcp::NetworkUser::Hash CharactersListRequest::getUserHash() const
{
    return userHash_;
}

const std::string& CharactersListRequest::getAccountId() const
{
    return accountId_;
}

}
}
}
