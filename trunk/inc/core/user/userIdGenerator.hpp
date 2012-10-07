#pragma once

#include <cstddef>
#include <queue>

#include <core.hpp>
#include <shared/types.hpp>

#ifdef WIN32
#pragma warning(disable: 4251)
#endif

namespace eMU {
namespace core {
namespace user {

const int16 invalidId_c = -1;

class eMU_CORE_DECLSPEC idGenerator_t {
public:
    idGenerator_t(size_t poolSize);

    int16 get();
    void insert(int16 id);

private:
    idGenerator_t();

    std::queue<int16> idsPool_;
};

}
}
}

#ifdef WIN32
#pragma warning(default: 4251)
#endif
