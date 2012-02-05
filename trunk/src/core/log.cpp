#include <boost/date_time/local_time/local_time.hpp>
#include "log.hpp"

std::string eMUCore::getCurrentDateString() {
    time_t rawtime = 0;
    time(&rawtime);

    std::tm *timeinfo = localtime(&rawtime);

    std::stringstream date;
    date  << std::setfill('0') << std::setw(2) << timeinfo->tm_mday
          << "/"
          << std::setfill('0') << std::setw(2) << timeinfo->tm_mon + 1
          << "/"
          << std::setfill('0') << std::setw(4) << timeinfo->tm_year + 1900
          << " :: "
          << std::setfill('0') << std::setw(2) << timeinfo->tm_hour
          << ":"
          << std::setfill('0') << std::setw(2) << timeinfo->tm_min
          << ":"
          << std::setfill('0') << std::setw(2) << timeinfo->tm_sec;

    return date.str();
}