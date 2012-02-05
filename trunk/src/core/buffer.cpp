#include "buffer.hpp"
#include "log.hpp"

eMUCore::network::readBuffer_t::readBuffer_t():
  payload_(maxPayloadSize_, 0) {}

void eMUCore::network::readBuffer_t::clear() {
    payload_.clear();
    payload_.resize(maxPayloadSize_, 0);
}

eMUCore::network::writeBuffer_t::writeBuffer_t():
  pending_(false),
  payload_(maxPayloadSize_, 0),
  payloadSize_(0),
  secPayload_(maxPayloadSize_, 0),
  secPayloadSize_(0) {}

void eMUCore::network::writeBuffer_t::clear() {
    this->clearPri();
    this->clearSec();
}

void eMUCore::network::writeBuffer_t::clearPri() {
    pending_ = false;

    payload_.clear();
    payload_.resize(maxPayloadSize_, 0);
    payloadSize_ = 0;
}

void eMUCore::network::writeBuffer_t::clearSec() {
    secPayload_.clear();
    secPayload_.resize(maxPayloadSize_, 0);
    secPayloadSize_ = 0;
}

bool eMUCore::network::writeBuffer_t::insert(const uint8 *payload, size_t size) {
    payload_t &destPayload = pending_ ? secPayload_ : payload_;
    size_t &destPayloadSize = pending_ ? secPayloadSize_ : payloadSize_;

    if((destPayloadSize + size) > maxPayloadSize_) {
        LOG_ERROR << "buffer overflow! pending: " << pending_ 
                  << ", current size: " << destPayloadSize 
                  << ", size: " << size << std::endl;
        return false;
    }

    memcpy(&destPayload[destPayloadSize], payload, size);
    destPayloadSize += size;

    return true;
}

void eMUCore::network::writeBuffer_t::swap() {
    payload_ = secPayload_;
    payloadSize_ = secPayloadSize_;
    this->clearSec();
}