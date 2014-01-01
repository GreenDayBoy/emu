#include <protocols/contexts/executive.hpp>

namespace eMU
{
namespace protocols
{
namespace contexts
{

Executive::~Executive() {}

core::transactions::Manager& Executive::getTransactionsManager()
{
    return transactionsManager_;
}

}
}
}
