#pragma once

#include <analyzer/payload/fields/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace payload
{
namespace fields
{

class Numeric: public Field
{
public:
    Numeric(QWidget *parent, size_t index, const std::string &value);

    void prepare();
};

}
}
}
}
