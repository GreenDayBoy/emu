#ifndef eMU_CORE_OBJECTSFACTORY_HPP
#define eMU_CORE_OBJECTSFACTORY_HPP

#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>

namespace eMU {
namespace core {

template<typename ObjectImpl>
class objectsFactory_t: private boost::noncopyable {
public:
    virtual ~objectsFactory_t() {}

protected:
    boost::object_pool<ObjectImpl> objectsPool_;
};

}
}

#endif