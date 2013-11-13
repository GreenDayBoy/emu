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

    bool isPending() const;
    void setPendingState();
    void clearPendingState();

    Payload& getPayload();
    const Payload& getPayload() const;

    Payload& getSecondPayload();
    const Payload& getSecondPayload() const;

private:
    void clearFirstPayload();
    void clearSecondPayload();

    Payload payload_;
    Payload secondPayload_;

    bool pending_;
};

}
}
}
