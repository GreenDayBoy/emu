#include <analyzer/views/writePayload.hpp>

#include <sstream>
#include <iomanip>
#include <boost/asio.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

void WritePayload::resize(size_t fieldsCount)
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

void WritePayload::insertFields(size_t fieldsCount)
{
    for(size_t i = fields_.size(); i < fieldsCount; ++i)
    {
        fields_.push_back(new Field(parent_, i, ""));
        fields_.back()->prepare();
    }
}

void WritePayload::removeFields(size_t fieldsCount)
{
    for(size_t i = fieldsCount; i < fields_.size(); ++i)
    {
        delete fields_[i];
    }

    fields_.resize(fieldsCount);
}

const FieldsContainer& WritePayload::getFields() const
{
    return fields_;
}

std::string WritePayload::getDump() const
{
    std::string dump;

    for(const auto field : fields_)
    {
        dump += field->getValueHex();
    }

    return dump;
}

}
}
}
