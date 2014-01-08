#pragma once

namespace eMU
{
namespace core
{
namespace common
{

class Transaction
{
public:
    virtual ~Transaction();

    void handle();

protected:
    virtual bool isValid() const = 0;

    virtual void handleValid() = 0;
    virtual void handleInvalid();
};

}
}
}
