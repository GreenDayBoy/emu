#pragma once

#include <analyzer/views/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

class StringField: public Field
{
public:
    StringField(QWidget *parent, size_t index, const std::string &value);

    void prepare();
};

}
}
}
