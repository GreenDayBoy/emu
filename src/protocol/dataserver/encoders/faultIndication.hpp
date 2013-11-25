#pragma once

#include <protocol/writeStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace encoders
{

class FaultIndication
{
public:
    FaultIndication(size_t clientHash, const std::string &message);

    const WriteStream& getWriteStream() const;

private:
    WriteStream writeStream_;
};

}
}
}
}
