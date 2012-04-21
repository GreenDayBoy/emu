#ifndef eMU_CORE_UDPWRITEBUFFERMANAGER_HPP
#define eMU_CORE_UDPWRITEBUFFERMANAGER_HPP

#include <boost/asio/ip/udp.hpp>
#include <map>
#include "objectsFactory.hpp"
#include "buffer.hpp"

#ifdef WIN32
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif

namespace eMU {
namespace core {
namespace network {
namespace udp {

class eMU_CORE_DECLSPEC writeBufferManager_t: public objectsFactory_t<writeBuffer_t> {
public:
    writeBuffer_t* get(const boost::asio::ip::udp::endpoint &endpoint);
    void free(const boost::asio::ip::udp::endpoint &endpoint);

private:
    std::map<boost::asio::ip::udp::endpoint, writeBuffer_t*> buffersMap_;
};

}
}
}
}

#ifdef WIN32
#pragma warning(default: 4275)
#pragma warning(default: 4251)
#endif

#endif