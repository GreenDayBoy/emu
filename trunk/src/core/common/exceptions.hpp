#pragma once

#include <core/common/exception.hpp>

namespace eMU
{
namespace core
{
namespace common
{
namespace exceptions
{

class MaxNumberOfUsersReachedException: public Exception {};

class UnknownUserException: public Exception {};

class EmptyXmlContentException: public Exception {};

class NotMatchedXmlNodeException: public Exception {};

}
}
}
}
