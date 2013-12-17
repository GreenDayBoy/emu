#include <analyzer/stream/views/readStreamView.hpp>
#include <analyzer/stream/fields/numericField.hpp>
#include <analyzer/stream/fields/stringField.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace views
{

void ReadStreamView::insertNumericField(const std::string &value)
{
    fields_.push_back(new fields::NumericField(parent_, fields_.size(), value));
    fields_.back()->prepare();
}

void ReadStreamView::insertStringField(const std::string &value)
{
    fields_.push_back(new fields::StringField(parent_, fields_.size(), value));
    fields_.back()->prepare();
}

}
}
}
}
