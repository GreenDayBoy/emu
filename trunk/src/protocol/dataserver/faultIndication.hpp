#pragma once

#include <protocol/readStream.hpp>
#include <protocol/writeStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{

class FaultIndication
{
public:
    FaultIndication(const ReadStream &readStream);
    FaultIndication(size_t clientHash, const std::string &message);

    const WriteStream& getWriteStream() const;
    size_t getClientHash() const;
    const std::string& getMessage() const;

private:
    ReadStream readStream_;
    WriteStream writeStream_;

    size_t clientHash_;
    std::string message_;
};

}
}
}
