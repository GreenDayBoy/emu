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

class String: public Field
{
public:
    String(QWidget *parent, size_t index, const std::string &value);

    void prepare();
};

}
}
}
}
