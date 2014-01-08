#pragma once

#include <analyzer/payload/fields/field.hpp>

#include <string>

namespace eMU
{
namespace analyzer
{
namespace payload
{

class Parser
{
public:
    virtual ~Parser();

    void parseAsNumeric(const std::string &value);
    void parseAsString(const std::string &value);
    void parseAsWideString(const std::string &value);

    void setParent(QWidget *parent);
    void clear();

private:
    QWidget *parent_;
    fields::Container fields_;
};

}
}
}
