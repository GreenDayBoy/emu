#ifndef eMU_CORE_LOG_HPP
#define eMU_CORE_LOG_HPP

#include <string>
#include <iostream>
#include "core.hpp"

namespace eMU {
namespace core {

eMU_CORE_DECLSPEC std::string getCurrentDateString();

}
}

#define LOG_BASE (std::cout << "<" << eMU::core::getCurrentDateString() << "> [" << __FILE__ << ":" << __LINE__ << "] ")
#define LOG_INFO (LOG_BASE << "[info] ")
#define LOG_WARNING (LOG_BASE << "[warning] ")
#define LOG_ERROR (LOG_BASE << "[error] ")
#define LOG_DEBUG (LOG_BASE << "[debug] ")

#endif