#include <analyzer/payload/fields/field.hpp>

#include <iomanip>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace analyzer
{
namespace payload
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

Field::~Field() {}

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

std::string Field::asHex() const
{
    std::string type = typeComboBox_.currentText().toStdString();
    std::string hex;

    if(type == kInt8Type)
    {
        hex = this->getHex<uint8_t>(boost::lexical_cast<uint16_t>(valueEdit_.text().toStdString()));
    }
    else if(type == kInt16Type)
    {
        hex = this->getHex(boost::lexical_cast<uint16_t>(valueEdit_.text().toStdString()));
    }
    else if(type == kInt32Type)
    {
        hex = this->getHex(boost::lexical_cast<uint32_t>(valueEdit_.text().toStdString()));
    }
    else if(type == kWStringType)
    {
        for(const char16_t c : valueEdit_.text().toStdWString())
        {
            hex += this->getHex(c);
        }
    }
    else if(type == kStringType)
    {
        for(const std::string::value_type c : valueEdit_.text().toStdString())
        {
            hex += this->getHex(c);
        }
    }

    return hex;
}

size_t Field::getSize() const
{
    std::string type = typeComboBox_.currentText().toStdString();

    if(type == kInt8Type)
    {
        return sizeof(uint8_t);
    }
    else if(type == kInt16Type)
    {
        return sizeof(uint16_t);
    }
    else if(type == kInt32Type)
    {
        return sizeof(uint32_t);
    }
    else if(type == kWStringType)
    {
        return valueEdit_.text().length() * sizeof(char16_t);
    }
    else if(type == kStringType)
    {
        return valueEdit_.text().length();
    }

    return 0;
}

}
}
}
}
