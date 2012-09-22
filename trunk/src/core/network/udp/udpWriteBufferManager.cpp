#include <network/udp/udpWriteBufferManager.hpp>
#include <common/log.hpp>

namespace eMU {
namespace core {
namespace network {
namespace udp {

writeBuffer_t* writeBufferManager_t::get(const boost::asio::ip::udp::endpoint &endpoint) {
    if(buffersMap_.end() != buffersMap_.find(endpoint)) {
        return buffersMap_[endpoint];
    }

    writeBuffer_t *wbuf = objectsPool_.construct();
    buffersMap_[endpoint] = wbuf;

    return wbuf;
}

void writeBufferManager_t::free(const boost::asio::ip::udp::endpoint &endpoint) {
    if(buffersMap_.end() != buffersMap_.find(endpoint)) {
        objectsPool_.destroy(buffersMap_[endpoint]);
        buffersMap_.erase(endpoint);
    } else {
        LOG_ERROR << "Wants to free buffer for unknown endpoint, address: " << endpoint.address() 
                  << ", port: " << endpoint.port() << std::endl;
    }
}

}
}
}
}