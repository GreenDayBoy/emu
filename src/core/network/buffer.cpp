#include <glog/logging.h>
#include <network/buffer.hpp>

namespace eMU {
namespace core {
namespace network {

ReadBuffer::ReadBuffer():
  payload_(kMaxPayloadSize, 0),
  payloadSize_(0) {}

void ReadBuffer::clear() {
    payload_.clear();
    payload_.resize(kMaxPayloadSize, 0);
    payloadSize_ = 0;
}

WriteBuffer::WriteBuffer():
  payload_(kMaxPayloadSize, 0),
  payloadSize_(0),
  pending_(false),
  secondPayload_(kMaxPayloadSize, 0),
  secondPayloadSize_(0) {}

void WriteBuffer::clear() {
    this->clearFirstPayload();
    this->clearSecondPayload();
}

void WriteBuffer::clearFirstPayload() {
    pending_ = false;

    payload_.clear();
    payload_.resize(kMaxPayloadSize, 0);
    payloadSize_ = 0;
}

void WriteBuffer::clearSecondPayload() {
    secondPayload_.clear();
    secondPayload_.resize(kMaxPayloadSize, 0);
    secondPayloadSize_ = 0;
}

bool WriteBuffer::insert(const Payload& payload) {
    Payload &destinationPayload = pending_ ? secondPayload_ : payload_;
    size_t &destinationPayloadSize = pending_ ? secondPayloadSize_ : payloadSize_;

    if((destinationPayloadSize + payload.size()) > kMaxPayloadSize) {
        LOG(INFO) << "buffer overflow! pending: " << pending_
                  << ", current size: " << destinationPayloadSize
                  << ", size: " << payload.size();

        return false;
    }

    memcpy(&destinationPayload[destinationPayloadSize], &payload[0], payload.size());
    destinationPayloadSize += payload.size();

    return true;
}

void WriteBuffer::swap() {
    payload_ = secondPayload_;
    payloadSize_ = secondPayloadSize_;
    this->clearSecondPayload();
}

}
}
}
