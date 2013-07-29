#pragma once

#include <core/common/exception.hpp>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{
namespace exceptions
{

class UnknownConnectionException: public common::Exception {};

class AlreadyExistingConnectionException: public common::Exception {};

}
}
}
}
}
