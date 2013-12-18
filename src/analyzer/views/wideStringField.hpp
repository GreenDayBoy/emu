#pragma once

#include <analyzer/views/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

class WideStringField: public Field
{
public:
    WideStringField(QWidget *parent, size_t index, const std::string &value);

    void prepare();
};

}
}
}
