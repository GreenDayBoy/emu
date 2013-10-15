#pragma once

#include <queue>
#include <memory>
#include <core/transactions/transaction.hpp>

namespace eMU
{
namespace core
{
namespace transactions
{

class Manager
{
public:
    typedef std::shared_ptr<Manager> Pointer;

    void queue(Transaction *transaction);
    bool dequeueAll();

private:
    std::queue<Transaction*> transactions_;
};

}
}
}
