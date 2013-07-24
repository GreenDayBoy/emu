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
    void handle();
    virtual ~Transaction();

protected:
    virtual bool validate() const;
    virtual void handleSelf() = 0;
};

}
}
}
