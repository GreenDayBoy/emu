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

class InvalidPacketSizeException: public common::Exception {};

class InvalidPacketHeaderException: public common::Exception {};

}
}
}
}
