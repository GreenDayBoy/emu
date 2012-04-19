#include "serviceThreading.hpp"

eMU::core::serviceThreading_t::serviceThreading_t(size_t maxNumOfThreads):
  maxNumOfThreads_(maxNumOfThreads) {}

eMU::core::serviceThreading_t::~serviceThreading_t() {
    threads_.interrupt_all();
}

void eMU::core::serviceThreading_t::initialize(boost::asio::io_service &ioService) {
    for(size_t i = 0; i < maxNumOfThreads_; ++i) {
        threads_.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
    }
}

void eMU::core::serviceThreading_t::join() {
    threads_.join_all();
}