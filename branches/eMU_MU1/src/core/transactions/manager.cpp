#include <core/transactions/manager.hpp>

namespace eMU
{
namespace core
{
namespace transactions
{

void Manager::queue(Transaction *transaction)
{
    transactions_.push(transaction);
}

bool Manager::dequeueAll()
{
    bool succeed = true;

    while(!transactions_.empty())
    {
        if(transactions_.front()->isValid())
        {
            transactions_.front()->handle();
        }
        else
        {
            succeed = false;
        }

        delete transactions_.front();
        transactions_.pop();
    }

    return succeed;
}

}
}
}
