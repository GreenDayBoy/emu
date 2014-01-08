#pragma once

#include <boost/log/trivial.hpp>

#define eMU_LOG(severity) BOOST_LOG_TRIVIAL(severity) << "[" << __FILE__ << ":" << __LINE__ << "] "
