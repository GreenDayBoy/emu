#include <glog/logging.h>
#include <connectserver/transactions/handshakeIndicationTransaction.hpp>

namespace eMU
{
namespace connectserver
{
namespace transactions
{

HandshakeIndicationTransaction::HandshakeIndicationTransaction(size_t hash, MessageSender &messageSender):
    hash_(hash),
    messageSender_(messageSender) {}

bool HandshakeIndicationTransaction::isValid() const
{
    return true;
}

void HandshakeIndicationTransaction::handle()
{
    LOG(INFO) << "hash: " << hash_ << ", sending handshake";

    messageSender_.sendHandshakeIndication(hash_);
}

}
}
}
