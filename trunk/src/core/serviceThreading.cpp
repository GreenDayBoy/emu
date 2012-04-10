#include "serviceThreading.hpp"

eMU::core::serviceThreading_t::serviceThreading_t(boost::asio::io_service &ioService, size_t maxNumOfThreads) {
    for(size_t i = 0; i < maxNumOfThreads; ++i) {
        threads_.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
    }
}

eMU::core::serviceThreading_t::~serviceThreading_t() {
    threads_.interrupt_all();
}

void eMU::core::serviceThreading_t::join() {
    threads_.join_all();
}