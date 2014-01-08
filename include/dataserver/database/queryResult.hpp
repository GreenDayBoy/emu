#pragma once

#include <dataserver/database/row.hpp>

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

class QueryResult {
public:
    Row& createRow(const Row::Fields &fields);
    const Rows& getRows() const;

private:
    Rows rows_;
};

}
}
}
