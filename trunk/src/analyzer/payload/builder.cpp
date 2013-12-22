#include <analyzer/payload/builder.hpp>

#include <sstream>
#include <iomanip>
#include <boost/asio.hpp>

namespace eMU
{
namespace analyzer
{
namespace payload
{

Builder::~Builder()
{
    this->clear();
}

void Builder::resize(size_t fieldsCount)
{
    if(fields_.size() > fieldsCount)
    {
        removeFields(fieldsCount);
    }
    else if(fields_.size() < fieldsCount)
    {
        insertFields(fieldsCount);
    }
}

void Builder::insertFields(size_t fieldsCount)
{
    for(size_t i = fields_.size(); i < fieldsCount; ++i)
    {
        fields_.push_back(new fields::Field(parent_, i, ""));
        fields_.back()->prepare();
    }
}

void Builder::removeFields(size_t fieldsCount)
{
    for(size_t i = fieldsCount; i < fields_.size(); ++i)
    {
        delete fields_[i];
    }

    fields_.resize(fieldsCount);
}

const fields::Container& Builder::getFields() const
{
    return fields_;
}

std::string Builder::getDump() const
{
    std::string dump;

    for(const auto field : fields_)
    {
        dump += field->asHex();
    }

    return dump;
}

size_t Builder::getFieldsSize() const
{
    size_t size = 0;

    for(const auto field : fields_)
    {
        size += field->getSize();
    }

    return size;
}

void Builder::setParent(QWidget *parent)
{
    parent_ = parent;
}

void Builder::clear()
{
    for(auto field : fields_)
    {
        delete field;
    }

    fields_.clear();
}

}
}
}
