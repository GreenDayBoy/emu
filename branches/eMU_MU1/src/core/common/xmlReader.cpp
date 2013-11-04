#include <core/common/xmlReader.hpp>

namespace eMU
{
namespace core
{
namespace common
{

XmlReader::XmlReader():
    currentNode_(nullptr) {}

void XmlReader::parse(std::string content, std::string firstNodeName)
{
    if(content.empty())
    {
        throw EmptyXmlContentException();
    }

    document_.parse<0>(const_cast<char*>(content.c_str()));
    currentNode_ = document_.first_node(const_cast<char*>(firstNodeName.c_str()));

    if(this->end())
    {
        throw NotMatchedXmlNodeException();
    }

    currentNode_ = currentNode_->first_node();
}

bool XmlReader::end()
{
    return currentNode_ == nullptr;
}

void XmlReader::next()
{
    if(!this->end())
    {
        currentNode_ = currentNode_->next_sibling();
    }
}

void XmlReader::clear()
{
    document_.clear();
}

}
}
}
