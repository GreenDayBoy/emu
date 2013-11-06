#pragma once

#include <core/network/payload.hpp>

namespace eMU
{
namespace core
{
namespace network
{

class WriteBuffer
{
public:
    WriteBuffer();

    void clear();
    bool insert(const Payload& payload);
    void swap();

    Payload payload_;
    Payload secondPayload_;

    bool pending_;

private:
    void clearFirstPayload();
    void clearSecondPayload();
};

}
}
}
