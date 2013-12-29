#include <core/common/xmlReader.hpp>

#include <fstream>

namespace eMU
{
namespace core
{
namespace common
{

XmlReader::XmlReader(const std::string &content):
    content_(content),
    currentNode_(nullptr) {}

void XmlReader::parse(std::string firstNodeName)
{
    if(content_.empty())
    {
        throw EmptyXmlContentException();
    }

    document_.parse<0>(const_cast<char*>(content_.c_str()));
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

std::string XmlReader::getXmlFileContent(const std::string &filePath)
{
    std::ifstream file(filePath);

    if(file.is_open())
    {
        std::stringstream content;
        content << file.rdbuf();
        file.close();

        return content.str();
    }

    return "";
}

}
}
}
