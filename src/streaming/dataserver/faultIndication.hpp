#pragma once

#include <streaming/readStream.hpp>
#include <streaming/writeStream.hpp>
#include <core/network/tcp/networkUser.hpp>

#include <string>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

class FaultIndication
{
public:
    FaultIndication(const ReadStream &readStream);
    FaultIndication(core::network::tcp::NetworkUser::Hash clientHash, const std::string &message);

    const WriteStream& getWriteStream() const;
    core::network::tcp::NetworkUser::Hash getClientHash() const;
    const std::string& getMessage() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    core::network::tcp::NetworkUser::Hash clientHash_;
    std::string message_;
};

}
}
}
