#pragma once

#include <map>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace dataserver
{
namespace database
{

class Row
{
public:
    typedef std::string Value;
    typedef std::vector<Value> Values;
    typedef std::map<std::string, size_t> Fields;

    Row(const Fields &fields);

    void insert(const Value &value);

    template<typename T>
    T getValue(const std::string &field) const
    {
        return this->getValue<T>(fields_.at(field));
    }

    template<typename T>
    T getValue(size_t index) const
    {
        if(index < values_.size())
        {
            Value v = values_[index];
            return boost::lexical_cast<T>(v);
        }
        else
        {
            return T();
        }
    }

private:
    Row();

    Values values_;
    Fields fields_;
};

typedef std::vector<Row> Rows;

}
}
}
