#pragma once

#include <queue>
#include <core/transactions/transaction.hpp>

namespace eMU {
namespace core {
namespace transactions {

class Manager {
public:
    void queue(Transaction *transaction);
    void dequeueAll();

private:
    std::queue<Transaction*> transactions_;
};

}
}
}
