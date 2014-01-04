#pragma once

#include <core/common/exception.hpp>

#include <vector>
#include <algorithm>
#include <boost/noncopyable.hpp>

namespace eMU
{
namespace core
{
namespace common
{

template<typename ObjectType>
class Factory: boost::noncopyable
{
public:
    typedef std::vector<ObjectType*> ObjectsContainer;

    class ObjectNotFoundException: public common::Exception {};

    virtual ~Factory()
    {
        for(auto object : objects_)
        {
            delete object;
        }
    }

    template<typename ParameterType>
    ObjectType& create(ParameterType &parameter)
    {
        objects_.push_back(new ObjectType(parameter));
        return *(objects_.back());
    }

    void destroy(const ObjectType &object)
    {
        typename ObjectsContainer::iterator it;
        it = this->getIterator(object);

        if(it != objects_.end())
        {
            delete *(it);
            objects_.erase(it);
        }
        else
        {
            throw ObjectNotFoundException();
        }
    }

    template<typename KeyType>
    ObjectType& find(const KeyType &key)
    {
        typename ObjectsContainer::iterator it;
        it = std::find_if(objects_.begin(),
                          objects_.end(),
                          [&key](const ObjectType *object) { return *object == key; });

        if(it == objects_.end())
        {
            throw ObjectNotFoundException();
        }

        return *(*it);
    }

    template<typename KeyType>
    bool exists(const KeyType &key) const
    {
        return std::find_if(objects_.begin(),
                            objects_.end(),
                            [&key](const ObjectType *object) { return *object == key; }) != objects_.end();
    }

    ObjectsContainer& getObjects()
    {
        return objects_;
    }

private:
    typename ObjectsContainer::iterator getIterator(const ObjectType &object)
    {
        typename ObjectsContainer::iterator it;
        it = std::find_if(objects_.begin(),
                          objects_.end(),
                          [&object](const ObjectType *objectFromVector) { return *objectFromVector == object; });

        return it;
    }

    ObjectsContainer objects_;
};

}
}
}
