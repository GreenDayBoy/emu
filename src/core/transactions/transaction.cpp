#include <core/transactions/transaction.hpp>

namespace eMU
{
namespace core
{
namespace transactions
{

Transaction::~Transaction()
{

}

void Transaction::handle()
{
    if(this->isValid())
    {
        this->handleValid();
    }
    else
    {
        this->handleInvalid();
    }
}

void Transaction::handleInvalid() {}

}
}
}
