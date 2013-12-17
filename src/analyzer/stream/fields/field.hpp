#pragma once

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace fields
{

class Field
{
public:
    static const std::string kInt8Type;
    static const std::string kInt16Type;
    static const std::string kInt32Type;
    static const std::string kWStringType;
    static const std::string kStringType;

    Field(QWidget *parent, size_t index, const std::string &value);
    virtual ~Field();

    virtual void prepare();
    std::string getType() const;
    std::string getValue() const;

protected:
    const size_t kHeight = 28;

    QComboBox typeComboBox_;
    QLineEdit valueEdit_;
    std::string value_;
};

typedef std::vector<Field*> FieldsContainer;

}
}
}
}
