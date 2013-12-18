#include <analyzer/views/payload.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

Payload::Payload():
    parent_(nullptr) {}

Payload::~Payload()
{
    clear();
}

void Payload::setParent(QWidget *parent)
{
    parent_ = parent;
}

void Payload::clear()
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
