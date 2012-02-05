#ifndef eMUCORE_LOG_HPP
#define eMUCORE_LOG_HPP

#include <string>
#include <iostream>
#include "core.hpp"

namespace eMUCore {
eMUCORE_DECLSPEC std::string getCurrentDateString();
};

#define LOG_BASE (std::cout << "<" << eMUCore::getCurrentDateString() << "> " << __FUNCTION__ << "():" << __LINE__ << " ")
#define LOG_INFO (LOG_BASE << "[info] ")
#define LOG_WARNING (LOG_BASE "[warning] ")
#define LOG_ERROR (LOG_BASE << "[error] ")
#define LOG_DEBUG (LOG_BASE << "[debug] ")

#endif //eMUCORE_LOG_HPP