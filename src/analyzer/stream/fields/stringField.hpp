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

class StringField: public Field
{
public:
    StringField(QWidget *parent, size_t index, const std::string &value);

    void prepare();
};

}
}
}
}
