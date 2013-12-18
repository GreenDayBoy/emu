#pragma once

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>

#include <sstream>
#include <iomanip>

namespace eMU
{
namespace analyzer
{
namespace views
{

class Field
{
public:
    Field(QWidget *parent, size_t index, const std::string &value);
    virtual ~Field();

    virtual void prepare();
    std::string getType() const;
    std::string getValueHex() const;
    size_t getSize() const;

protected:
    static const std::string kInt8Type;
    static const std::string kInt16Type;
    static const std::string kInt32Type;
    static const std::string kWStringType;
    static const std::string kStringType;
    const size_t kHeight = 28;

    template<typename T>
    std::string dumpValue(const T &value) const
    {
        std::stringstream s;

        const uint8_t *array = reinterpret_cast<const uint8_t*>(&value);

        for(size_t i = 0; i < sizeof(value); ++i)
        {
            s << std::hex << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(array[i]) << " ";
        }

        return s.str();
    }

    QComboBox typeComboBox_;
    QLineEdit valueEdit_;
    std::string value_;
};

typedef std::vector<Field*> FieldsContainer;

}
}
}
