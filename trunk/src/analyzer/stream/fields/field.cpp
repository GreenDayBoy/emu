#include <analyzer/stream/fields/field.hpp>

#include <sstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace fields
{

const std::string Field::kInt8Type = "int8_t";
const std::string Field::kInt16Type = "int16_t";
const std::string Field::kInt32Type = "int32_t";
const std::string Field::kWStringType = "wstring";
const std::string Field::kStringType = "string";

Field::Field(QWidget *parent, size_t index, const std::string &value):
    typeComboBox_(parent),
    valueEdit_(parent),
    value_(value)
{
    typeComboBox_.setGeometry(QRect(80, 3 + index * kHeight, 140, 25));
    typeComboBox_.show();

    valueEdit_.setGeometry(QRect(230, 3 + index * kHeight, 310, 25));
    valueEdit_.show();
}

void Field::prepare()
{
    typeComboBox_.insertItem(0, QString::fromStdString(kInt8Type));
    typeComboBox_.insertItem(1, QString::fromStdString(kInt16Type));
    typeComboBox_.insertItem(2, QString::fromStdString(kInt32Type));
    typeComboBox_.insertItem(3, QString::fromStdString(kStringType));
    typeComboBox_.insertItem(4, QString::fromStdString(kWStringType));

    valueEdit_.setText("0");
}

std::string Field::getType() const
{
    return typeComboBox_.currentText().toStdString();
}

std::string Field::getValue() const
{
    return valueEdit_.text().toStdString();
}

Field::~Field() {}

}
}
}
}
