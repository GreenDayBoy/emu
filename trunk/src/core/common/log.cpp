#include <iomanip>
#include <sstream>
#include <ctime>

#include <common/log.hpp>

std::string eMU::core::common::getCurrentDateString() {
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