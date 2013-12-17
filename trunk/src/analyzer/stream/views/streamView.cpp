#include <analyzer/stream/views/streamView.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace views
{

StreamView::StreamView():
    parent_(nullptr) {}

StreamView::~StreamView()
{
    clear();
}

void StreamView::setParent(QWidget *parent)
{
    parent_ = parent;
}

void StreamView::clear()
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
}
