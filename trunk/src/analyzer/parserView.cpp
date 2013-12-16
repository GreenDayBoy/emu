#include <analyzer/parserView.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace analyzer
{

ParserView::ParserView():
    parent_(nullptr) {}

ParserView::~ParserView()
{
    for(auto field : fields_)
    {
        delete field;
    }

    fields_.clear();
}

void ParserView::setParent(QWidget *parent)
{
    parent_ = parent;
}

void ParserView::resize(size_t fieldsCount)
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

void ParserView::insertFields(size_t fieldsCount)
{
    for(size_t i = fields_.size(); i < fieldsCount; ++i)
    {
        fields_.push_back(new Field(parent_, i));
    }
}

void ParserView::removeFields(size_t fieldsCount)
{
    for(size_t i = fieldsCount; i < fields_.size(); ++i)
    {
        delete fields_[i];
    }

    fields_.resize(fieldsCount);
}

}
}
