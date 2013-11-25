#include <dataserver/database/row.hpp>

namespace eMU
{
namespace dataserver
{
namespace database
{

Row::Row(const Fields &fields):
    fields_(fields) {}

void Row::insert(const Value &value)
{
    values_.push_back(value);
}

template<>
std::string Row::getValue(size_t index) const
{
    if(index < values_.size())
    {
        return values_[index];
    }
    else
    {
        return "";
    }
}

}
}
}
