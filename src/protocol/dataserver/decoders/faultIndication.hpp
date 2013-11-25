#pragma once

#include <protocol/readStream.hpp>

#include <string>

namespace eMU
{
namespace protocol
{
namespace dataserver
{
namespace decoders
{

class FaultIndication
{
public:
    FaultIndication(const ReadStream &readStream);

    size_t getClientHash() const;
    const std::string& getMessage() const;

private:
    ReadStream readStream_;

    size_t clientHash_;
    std::string message_;
};

}
}
}
}
