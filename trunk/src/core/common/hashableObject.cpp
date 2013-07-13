#include <common/hashableObject.hpp>

namespace eMU {
namespace core {
namespace common {

HashableObject::~HashableObject() {}

size_t HashableObject::hash() const {
    return reinterpret_cast<size_t>(this);
}

}
}
}
