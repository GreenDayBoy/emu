#pragma once

#include <core/common/exception.hpp>

namespace eMU
{
namespace core
{
namespace protocol
{
namespace exceptions
{

class EmptyPayloadException: public common::Exception {};

class InvalidMessageSizeException: public common::Exception {};

class InvalidMessageHeaderException: public common::Exception {};

}
}
}
}
