#pragma once

#include <analyzer/stream/fields/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace views
{

class StreamView
{
public:
    StreamView();
    virtual ~StreamView();

    void setParent(QWidget *parent);
    void clear();

protected:
    QWidget *parent_;
    fields::FieldsContainer fields_;
};

}
}
}
}
