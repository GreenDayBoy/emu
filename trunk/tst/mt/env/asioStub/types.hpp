#pragma once

#include <functional>
#include <boost/system/error_code.hpp>

namespace eMU
{
namespace mt
{
namespace env
{
namespace asioStub
{

typedef std::function<void(const boost::system::error_code, size_t)> IoHandler;
typedef std::function<void(const boost::system::error_code&)> AcceptHandler;

}
}
}
}
