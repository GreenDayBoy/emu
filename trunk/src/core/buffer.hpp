#ifndef eMU_CORE_BUFFER_HPP
#define eMU_CORE_BUFFER_HPP

#include <vector>
#include "core.hpp"
#include "../shared/types.hpp"

#ifdef WIN32
#pragma warning(disable: 4251)
#endif

namespace eMU {
namespace core {
namespace network {

typedef std::vector<uint8> payload_t;
const size_t maxPayloadSize_c = 4096;

class eMU_CORE_DECLSPEC readBuffer_t {
public:
    readBuffer_t();
    void clear();

    payload_t payload_;
};

class eMU_CORE_DECLSPEC writeBuffer_t {
public:
    writeBuffer_t();
    void clear();
    bool insert(const uint8 *payload, size_t size);
    void swap();

    payload_t payload_;
    size_t payloadSize_;
    bool pending_;
    payload_t secPayload_;
    size_t secPayloadSize_;

private:
    void clearPri();
    void clearSec();
};

}
}
}

#ifdef WIN32
#pragma warning(default: 4251)
#endif

#endif