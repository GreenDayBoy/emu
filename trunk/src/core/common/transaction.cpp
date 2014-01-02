#include <core/common/transaction.hpp>

namespace eMU
{
namespace core
{
namespace common
{

Transaction::~Transaction()
{

}

void Transaction::handle()
{
    if(this->isValid())
    {
        this->handleValid();
    }
    else
    {
        this->handleInvalid();
    }
}

void Transaction::handleInvalid() {}

}
}
}
