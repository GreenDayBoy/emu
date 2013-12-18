#include <analyzer/stream/fields/numericField.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace fields
{

NumericField::NumericField(QWidget *parent, size_t index, const std::string &value):
    Field(parent, index, value) {}

void NumericField::prepare()
{
    value_.erase(std::remove(value_.begin(), value_.end(), ' '), value_.end());

    std::stringstream ss;
    ss << std::hex << value_;
    size_t dec = 0;
    ss >> dec;

    switch(value_.length() / 2)
    {
    case 1:
        typeComboBox_.insertItem(0, QString::fromStdString(kInt8Type));
        valueEdit_.setText(QString::fromStdString(boost::lexical_cast<std::string>(dec)));
        break;
    case 2:
        typeComboBox_.insertItem(0, QString::fromStdString(kInt16Type));
        valueEdit_.setText(QString::fromStdString(boost::lexical_cast<std::string>(boost::asio::detail::socket_ops::host_to_network_short(dec))));
        break;
    case 4:
        typeComboBox_.insertItem(0, QString::fromStdString(kInt32Type));
        valueEdit_.setText(QString::fromStdString(boost::lexical_cast<std::string>(boost::asio::detail::socket_ops::host_to_network_long(dec))));
        break;
    default:
        typeComboBox_.insertItem(0, "<unknown>");
        valueEdit_.setText(QString::fromStdString(value_));
        break;
    }
}

}
}
}
}
