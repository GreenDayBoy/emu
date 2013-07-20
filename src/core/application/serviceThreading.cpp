#include <core/application/serviceThreading.hpp>

namespace eMU {
namespace core {
namespace application {

ServiceThreading::ServiceThreading(size_t maxNumberOfThreads):
  maxNumberOfThreads_(maxNumberOfThreads) {}

ServiceThreading::~ServiceThreading() {
    threads_.interrupt_all();
}

void ServiceThreading::initialize() {
    for(size_t i = 0; i < maxNumberOfThreads_; ++i) {
        threads_.create_thread(std::bind(static_cast<size_t (asio::io_service::*)()>(&asio::io_service::run), &ioService_));
    }
}

void ServiceThreading::join() {
    threads_.join_all();
}

void ServiceThreading::stop() {
    ioService_.stop();
}

asio::io_service& ServiceThreading::ioService() {
    return ioService_;
}

}
}
}
