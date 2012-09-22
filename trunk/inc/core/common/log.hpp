#ifndef __core_log_hpp__
#define __core_log_hpp__

#include <string>
#include <iostream>

#include <core.hpp>

namespace eMU {
namespace core {
namespace common {

eMU_CORE_DECLSPEC std::string getCurrentDateString();

}
}
}

#define LOG_BASE (std::cout << "<" << eMU::core::common::getCurrentDateString() << "> [" << __FILE__ << ":" << __LINE__ << "] ")
#define LOG_INFO (LOG_BASE << "[info] ")
#define LOG_WARNING (LOG_BASE << "[warning] ")
#define LOG_ERROR (LOG_BASE << "[error] ")
#define LOG_DEBUG (LOG_BASE << "[debug] ")

#endif