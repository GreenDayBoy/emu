#pragma once

#include <core/common/exception.hpp>

namespace eMU
{
namespace connectserver
{
namespace exceptions
{

class InvalidProtocolIdException: public core::common::Exception {};

class UnknownMessageException: public core::common::Exception {};

class TooShortMessageException: public core::common::Exception {};

}
}
}
