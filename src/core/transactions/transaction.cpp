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
    if(validate())
    {
        this->handleSelf();
    }
}

bool Transaction::validate() const
{
    return true;
}

}
}
}
