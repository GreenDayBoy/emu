#include <glog/logging.h>
#include <core/network/writeBuffer.hpp>

namespace eMU
{
namespace core
{
namespace network
{

WriteBuffer::WriteBuffer():
    pending_(false) {}

void WriteBuffer::clear()
{
    this->clearFirstPayload();
    this->clearSecondPayload();
}

void WriteBuffer::clearFirstPayload()
{
    pending_ = false;

    payload_.clear();
}

void WriteBuffer::clearSecondPayload()
{
    secondPayload_.clear();
}

bool WriteBuffer::insert(const Payload& payload)
{
    Payload &destinationPayload = pending_ ? secondPayload_ : payload_;

    if((destinationPayload.getSize() + payload.getSize()) > Payload::getMaxSize())
    {
        LOG(ERROR) << "buffer overflow! pending: " << pending_
                   << ", current size: " << payload.getSize();

        return false;
    }

    memcpy(&destinationPayload[destinationPayload.getSize()], &payload[0], payload.getSize());
    destinationPayload.setSize(destinationPayload.getSize() + payload.getSize());

    return true;
}

void WriteBuffer::swap()
{
    payload_ = secondPayload_;
    this->clearSecondPayload();
}

bool WriteBuffer::isPending() const
{
    return pending_;
}

void WriteBuffer::setPendingState()
{
    pending_ = true;
}

void WriteBuffer::clearPendingState()
{
    pending_ = false;
}

Payload& WriteBuffer::getPayload()
{
    return payload_;
}

const Payload& WriteBuffer::getPayload() const
{
    return payload_;
}

Payload& WriteBuffer::getSecondPayload()
{
    return secondPayload_;
}

const Payload& WriteBuffer::getSecondPayload() const
{
    return secondPayload_;
}

}
}
}
