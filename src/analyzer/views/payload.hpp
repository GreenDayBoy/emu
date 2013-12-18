#pragma once

#include <analyzer/views/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

class Payload
{
public:
    Payload();
    virtual ~Payload();

    void setParent(QWidget *parent);
    void clear();

protected:
    QWidget *parent_;
    FieldsContainer fields_;
};

}
}
}
