#include "userIdGenerator.hpp"

eMU::core::user::idGenerator_t::idGenerator_t(size_t poolSize) {
    for(size_t i = 0; i < poolSize; ++i) {
        this->insert(static_cast<int16>(i));
    }
}

int16 eMU::core::user::idGenerator_t::get() {
    if(!pool_.empty()) {
        int16 id = pool_.front();
        pool_.pop();
        return id;
    }
    
    return invalidId_c;
}

void eMU::core::user::idGenerator_t::insert(int16 id) {
    pool_.push(id);
}