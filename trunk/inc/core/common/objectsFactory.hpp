#ifndef __core_objectsFactory_hpp__
#define __core_objectsFactory_hpp__

#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>

namespace eMU {
namespace core {

template<typename objectImpl>
class objectsFactory_t: private boost::noncopyable {
public:
    virtual ~objectsFactory_t() {}

protected:
    boost::object_pool<objectImpl> objectsPool_;
};

}
}

#endif