#include <core/transactions/manager.hpp>

namespace eMU {
namespace core {
namespace transactions {

void Manager::queue(Transaction *transaction) {
    transactions_.push(transaction);
}

void Manager::dequeueAll() {
    while(!transactions_.empty()) {
        transactions_.front()->handle();
        delete transactions_.front();

        transactions_.pop();
    }
}

}
}
}
