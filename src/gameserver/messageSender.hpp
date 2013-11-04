#pragma once

#include <core/network/buffer.hpp>
#include <common/mockable.hpp>
#include <functional>

namespace eMU
{
namespace gameserver
{

class MessageSender
{
public:
    typedef std::function<void(size_t, const core::network::Payload&)> SendFunctor;

    MessageSender(const SendFunctor &sendFunctor);
    virtual ~MessageSender();

private:
    MessageSender();

    SendFunctor sendFunctor_;
};

}
}
