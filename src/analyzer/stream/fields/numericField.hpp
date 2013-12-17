#pragma once

#include <analyzer/stream/fields/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace fields
{

class NumericField: public Field
{
public:
    NumericField(QWidget *parent, size_t index, const std::string &value);

    void prepare();
};

}
}
}
}
