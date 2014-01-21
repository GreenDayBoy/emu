#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <core/network/tcp/networkUser.hpp>
#include <streaming/common/characterViewInfo.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

class CharacterCreateRequest
{
public:
    CharacterCreateRequest(const ReadStream &readStream);
    CharacterCreateRequest(core::network::tcp::NetworkUser::Hash userHash,
                           const std::string &accountId,
                           const common::CharacterViewInfo &characterCreateInfo);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getUserHash() const;
    const std::string& getAccountId() const;
    const common::CharacterViewInfo& getCharacterCreateInfo() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash userHash_;
    std::string accountId_;
    common::CharacterViewInfo characterCreateInfo_;
};

}
}
}
