#include <analyzer/stream/views/writeStreamView.hpp>

#include <sstream>
#include <iomanip>
#include <boost/asio.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace views
{

void WriteStreamView::resize(size_t fieldsCount)
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

void WriteStreamView::insertFields(size_t fieldsCount)
{
    for(size_t i = fields_.size(); i < fieldsCount; ++i)
    {
        fields_.push_back(new fields::Field(parent_, i, ""));
        fields_.back()->prepare();
    }
}

void WriteStreamView::removeFields(size_t fieldsCount)
{
    for(size_t i = fieldsCount; i < fields_.size(); ++i)
    {
        delete fields_[i];
    }

    fields_.resize(fieldsCount);
}

const fields::FieldsContainer& WriteStreamView::getFields() const
{
    return fields_;
}

}
}
}
}
