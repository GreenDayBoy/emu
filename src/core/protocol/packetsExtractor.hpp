#pragma once

#include <stdint.h>
#include <core/network/buffer.hpp>

namespace eMU
{
namespace core
{
namespace protocol
{

class PacketsExtractor
{
public:
    typedef std::vector<network::Payload> PayloadsContainer;

    PacketsExtractor(const network::Payload &payload);

    void extract();
    const PayloadsContainer& payloads() const;

private:
    PacketsExtractor();

    const network::Payload &payload_;
    PayloadsContainer payloads_;
};

}
}
}
