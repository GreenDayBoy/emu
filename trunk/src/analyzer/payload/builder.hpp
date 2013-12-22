#pragma once

#include <analyzer/payload/fields/field.hpp>

namespace eMU
{
namespace analyzer
{
namespace payload
{

class Builder
{
public:
    virtual ~Builder();

    void resize(size_t fieldsCount);
    const fields::Container& getFields() const;
    std::string getDump() const;
    size_t getFieldsSize() const;

    void setParent(QWidget *parent);
    void clear();

private:
    void insertFields(size_t fieldsCount);
    void removeFields(size_t fieldsCount);

    QWidget *parent_;
    fields::Container fields_;
};

}
}
}
