#pragma once

#include <vector>
#include <stdint.h>
#include <cstdlib>

namespace eMU {
namespace core {
namespace network {

typedef std::vector<uint8_t> Payload;
const size_t kMaxPayloadSize = 4096;

class ReadBuffer {
public:
    ReadBuffer();

    void clear();

    Payload payload_;
    size_t payloadSize_;
};

class WriteBuffer {
public:
    WriteBuffer();

    void clear();
    bool insert(const Payload& payload);
    void swap();

    Payload payload_;
    size_t payloadSize_;

    bool pending_;

    Payload secondPayload_;
    size_t secondPayloadSize_;

private:
    void clearFirstPayload();
    void clearSecondPayload();
};

}
}
}
