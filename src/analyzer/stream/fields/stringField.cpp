#include <analyzer/stream/fields/stringField.hpp>

#include <sstream>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace fields
{

StringField::StringField(QWidget *parent, size_t index, const std::string &value):
    Field(parent, index, value) {}

void StringField::prepare()
{
    value_.erase(std::remove(value_.begin(), value_.end(), ' '));

    std::string str;

    for(size_t i = 0; i < value_.length(); i += 4)
    {
        std::string c(value_, i, 2);

        std::stringstream ss;
        ss << std::hex << c;
        size_t dec = 0;
        ss >> dec;

        str.push_back(dec);
    }

    typeComboBox_.insertItem(0, QString::fromStdString(kWStringType));
    valueEdit_.setText(QString::fromStdString(str));
}

}
}
}
}
