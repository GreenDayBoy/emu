#include <core/common/serviceThreading.hpp>

namespace eMU {
namespace core {
namespace common {

ServiceThreading::ServiceThreading(size_t maxNumberOfThreads):
  maxNumberOfThreads_(maxNumberOfThreads) {}

ServiceThreading::~ServiceThreading() {
    threads_.interrupt_all();
}

void ServiceThreading::initialize() {
    for(size_t i = 0; i < maxNumberOfThreads_; ++i) {
        threads_.create_thread(boost::bind(&asio::io_service::run, &ioService_));
    }
}

void ServiceThreading::join() {
    threads_.join_all();
}

}
}
}
