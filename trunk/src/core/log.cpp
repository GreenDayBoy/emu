#include <iomanip>
#include <time.h>
#include <sstream>
#include "log.hpp"

std::string eMU::core::getCurrentDateString() {
    time_t t = 0;
    time(&t);

    std::tm *timeInfo = localtime(&t);

    std::stringstream date;
    date  << std::setfill('0') << std::setw(2) << timeInfo->tm_mday
          << "/"
          << std::setfill('0') << std::setw(2) << timeInfo->tm_mon + 1
          << "/"
          << std::setfill('0') << std::setw(4) << timeInfo->tm_year + 1900
          << " :: "
          << std::setfill('0') << std::setw(2) << timeInfo->tm_hour
          << ":"
          << std::setfill('0') << std::setw(2) << timeInfo->tm_min
          << ":"
          << std::setfill('0') << std::setw(2) << timeInfo->tm_sec;

    return date.str();
}