#pragma once

#include <mt/env/asioStub/ioService.hpp>
#include <core/network/buffer.hpp>

namespace eMU {
namespace mt {
namespace env {
namespace networkStub {

class Stub {
public:
    size_t createConnection();

    void send(size_t hash, const eMU::core::network::Payload &payload);
    eMU::core::network::Payload receive(size_t hash);

    void sendTo(const eMU::core::network::Payload &payload);
    eMU::core::network::Payload receiveFrom();

    void sendToClientSocket(const eMU::core::network::Payload &payload);
    eMU::core::network::Payload receiveFromClientSocket();

private:
    asioStub::io_service service_;
};

}
}
}
}
