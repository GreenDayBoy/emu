#pragma once

#include <analyzer/views/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
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
