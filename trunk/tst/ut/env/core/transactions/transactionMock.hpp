#pragma once

#include <gmock/gmock.h>
#include <core/transactions/transaction.hpp>

namespace eMU
{
namespace ut
{
namespace env
{
namespace core
{
namespace transactions
{

class TransactionMock: public eMU::core::transactions::Transaction
{
public:
    MOCK_CONST_METHOD0(isValid, bool());
    MOCK_METHOD0(handle, void());
};

}
}
}
}
}
