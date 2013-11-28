#pragma once

#include <core/common/hashableObject.hpp>
#include <string>

namespace eMU
{
namespace loginserver
{

class User: public eMU::core::common::HashableObject
{
public:
    void setAccountId(const std::string &accountId);
    const std::string& getAccountId() const;

private:
    std::string accountId_;
};

}
}
