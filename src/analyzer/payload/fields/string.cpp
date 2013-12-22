#include <analyzer/payload/fields/string.hpp>

#include <sstream>

namespace eMU
{
namespace analyzer
{
namespace payload
{
namespace fields
{

String::String(QWidget *parent, size_t index, const std::string &value):
    Field(parent, index, value)
{
    valueEdit_.setReadOnly(true);
}

void String::prepare()
{
    value_.erase(std::remove(value_.begin(), value_.end(), ' '), value_.end());

    std::string str;

    for(size_t i = 0; i < value_.length(); i += 2)
    {
        std::string c(value_, i, 2);

        std::stringstream ss;
        ss << std::hex << c;
        size_t dec = 0;
        ss >> dec;

        str.push_back(dec);
    }

    typeComboBox_.insertItem(0, QString::fromStdString(kStringType));
    valueEdit_.setText(QString::fromStdString(str));
}

}
}
}
}
