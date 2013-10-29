#pragma once

#include <connectserver/messageSender.hpp>
#include <core/transactions/manager.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

class HandshakeIndicationTransaction: public eMU::core::transactions::Transaction
{
public:
    HandshakeIndicationTransaction(size_t hash, MessageSender &messageSender);

    bool isValid() const;
    void handle();

private:
    size_t hash_;
    MessageSender &messageSender_;
};

}
}
}
