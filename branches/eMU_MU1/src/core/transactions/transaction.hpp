#pragma once

namespace eMU
{
namespace core
{
namespace transactions
{

class Transaction
{
public:
    virtual ~Transaction();

    virtual void handle() = 0;
    virtual bool isValid() const = 0;
};

}
}
}
