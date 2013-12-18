#include <analyzer/views/readPayload.hpp>
#include <analyzer/views/numericField.hpp>
#include <analyzer/views/stringField.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

void ReadPayload::insertNumericField(const std::string &value)
{
    fields_.push_back(new NumericField(parent_, fields_.size(), value));
    fields_.back()->prepare();
}

void ReadPayload::insertStringField(const std::string &value)
{
    fields_.push_back(new StringField(parent_, fields_.size(), value));
    fields_.back()->prepare();
}

}
}
}
