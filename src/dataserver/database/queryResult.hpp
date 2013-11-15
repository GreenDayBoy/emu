#pragma once

#include <boost/noncopyable.hpp>
#include <map>
#include <vector>
#include <string>

namespace eMU
{
namespace dataserver
{
namespace database
{

class QueryResult: boost::noncopyable {
public:
    typedef std::map<std::string, size_t> Fields;
    typedef std::vector<std::string> Row;
    typedef std::vector<Row> Rows;

    ~QueryResult();

    template<typename T>
    T getFieldValue(const std::string &fieldName) const
    {
        return T();
    }

    template<typename T>
    T getFieldValue(size_t index) const
    {
        return T();
    }

    Fields& getFields();
    const Fields& getFields() const;

    Rows& getRows();
    const Rows& getRows() const;

private:
    Fields fields_;
    Rows rows_;
};

}
}
}
