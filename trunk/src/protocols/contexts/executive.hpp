#pragma once

#include <core/transactions/manager.hpp>
#include <boost/noncopyable.hpp>

namespace eMU
{
namespace protocols
{
namespace contexts
{

class Executive: boost::noncopyable
{
public:
    virtual ~Executive();

    core::transactions::Manager& getTransactionsManager();

protected:
    core::transactions::Manager transactionsManager_;
};

}
}
}
