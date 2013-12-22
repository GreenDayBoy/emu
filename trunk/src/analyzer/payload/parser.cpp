#include <analyzer/payload/parser.hpp>
#include <analyzer/payload/fields/numeric.hpp>
#include <analyzer/payload/fields/string.hpp>
#include <analyzer/payload/fields/wideString.hpp>

namespace eMU
{
namespace analyzer
{
namespace payload
{

Parser::~Parser()
{
    this->clear();
}

void Parser::parseAsNumeric(const std::string &value)
{
    fields_.push_back(new fields::Numeric(parent_, fields_.size(), value));
    fields_.back()->prepare();
}

void Parser::parseAsString(const std::string &value)
{
    fields_.push_back(new fields::String(parent_, fields_.size(), value));
    fields_.back()->prepare();
}

void Parser::parseAsWideString(const std::string &value)
{
    fields_.push_back(new fields::WideString(parent_, fields_.size(), value));
    fields_.back()->prepare();
}

void Parser::setParent(QWidget *parent)
{
    parent_ = parent;
}

void Parser::clear()
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
