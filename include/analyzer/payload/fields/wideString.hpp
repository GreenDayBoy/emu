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

class WideString: public Field
{
public:
    WideString(QWidget *parent, size_t index, const std::string &value);

    void prepare();
};

}
}
}
}
