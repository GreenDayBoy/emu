#include <dataserver/database/queryResult.hpp>

namespace eMU
{
namespace dataserver
{
namespace database
{

QueryResult::~QueryResult() {}

QueryResult::Fields& QueryResult::getFields()
{
    return fields_;
}

const QueryResult::Fields& QueryResult::getFields() const
{
    return fields_;
}

QueryResult::Rows& QueryResult::getRows()
{
    return rows_;
}

const QueryResult::Rows& QueryResult::getRows() const
{
    return rows_;
}

}
}
}
