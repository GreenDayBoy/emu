#pragma once

#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>

namespace eMU {
namespace core {

template<typename objectImpl>
class objectsFactory_t: private boost::noncopyable {
public:
    virtual ~objectsFactory_t() {}

protected:
    typename boost::object_pool<objectImpl> objectsPool_;
};

}
}
