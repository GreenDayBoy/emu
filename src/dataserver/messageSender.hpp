#pragma once

#include <core/network/payload.hpp>
#include <common/mockable.hpp>

#include <functional>
#include <stdint.h>
#include <cstddef>

namespace eMU
{
namespace dataserver
{

class MessageSender
{
public:
    typedef std::function<void(size_t, const core::network::Payload&)> SendFunctor;

    MessageSender(const SendFunctor &sendFunctor);
    virtual ~MessageSender();

    MOCKABLE void sendCheckAccountResponse(size_t hash, uint32_t checkResult);

private:
    MessageSender();

    SendFunctor sendFunctor_;

};

}
}
