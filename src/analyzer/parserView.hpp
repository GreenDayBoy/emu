#pragma once

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>

namespace eMU
{
namespace analyzer
{

class ParserView
{
public:
    class Field
    {
    public:
        Field(QWidget *parent, size_t index):
            type_(parent),
            value_(parent)
        {
            type_.insertItem(0, "int8_t");
            type_.insertItem(1, "int16_t");
            type_.insertItem(2, "int32_t");
            type_.insertItem(3, "int64_t");
            type_.insertItem(4, "string");
            type_.insertItem(5, "wstring");

            type_.setGeometry(QRect(80, 60 + index * kHeight, 140, 25));
            value_.setGeometry(QRect(230, 60 + index * kHeight, 310, 25));

            type_.show();
            value_.show();
        }

    private:
        const size_t kHeight = 28;

        QComboBox type_;
        QLineEdit value_;
    };

    typedef std::vector<Field*> Fields;

    ParserView();
    virtual ~ParserView();
    void resize(size_t fieldsCount);
    void setParent(QWidget *parent);

private:
    void insertFields(size_t fieldsCount);
    void removeFields(size_t fieldsCount);

    QWidget *parent_;
    Fields fields_;

};

}
}
