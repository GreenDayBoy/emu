#include <dataserver/database/queryResult.hpp>

namespace eMU
{
namespace dataserver
{
namespace database
{

const Rows& QueryResult::getRows() const
{
    return rows_;
}

Row& QueryResult::createRow(const Row::Fields &fields)
{
    rows_.push_back(Row(fields));

    return rows_.back();
}

}
}
}
