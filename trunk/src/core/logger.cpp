#include <iomanip>
#include <iostream>
#include "core.h"

using namespace eMUCore;

const std::string eMUCore::logger_t::c_loggerMessageHeader[]	= {"/DBG/ ",
        "/INF/ ",
        "/WRN/ ",
        "/ERR/ ",
        "/PRT/ "
                                                               };

const unsigned char eMUCore::logger_t::c_loggerMessageColor[] = {11,
        2,
        14,
        12,
        5
                                                                };

logger_t::logger_t():
    m_stdOutput(INVALID_HANDLE_VALUE) {}

void logger_t::startup() {
    this->openFile();

    m_stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    if(m_stdOutput == INVALID_HANDLE_VALUE) {
        exception_t e;
        e.in() << __FILE__ << ":" << __LINE__ << " :: Could not get standard output handle."
               << " GetStdHandle failed with error #" << GetLastError() << ".";
        throw e;
    }
}

void logger_t::cleanup() {
    m_file.close();
}

void logger_t::openFile() {
    GetLocalTime(&m_openFileTime);
    std::stringstream fileName;

    fileName << "logs\\"
             << std::setfill('0') << std::setw(4) << m_openFileTime.wYear
             << "_"
             << std::setfill('0') << std::setw(2) << m_openFileTime.wMonth
             << "_"
             << std::setfill('0') << std::setw(2) << m_openFileTime.wDay
             << ".log";

    if(m_file.is_open()) {
        m_file.close();
    }

    m_file.open(fileName.str().c_str(), std::ios::app);

    if(!m_file.is_open()) {
        exception_t e;
        e.in() << __FILE__ << ":" << __LINE__ << " :: Could not open file: " << fileName.str() << ".";
        throw e;
    }
}

std::stringstream& logger_t::in(loggerMessage_e::type_t type) {
    m_synchronizer.lock();

    SYSTEMTIME localTime = {0};
    GetLocalTime(&localTime);

    if(localTime.wDay != m_openFileTime.wDay) {
        this->openFile();
    }

    m_date << "["
           << std::setfill('0') << std::setw(2) << localTime.wDay
           << "/"
           << std::setfill('0') << std::setw(2) << localTime.wMonth
           << "/"
           << std::setfill('0') << std::setw(4) << localTime.wYear
           << " :: "
           << std::setfill('0') << std::setw(2) << localTime.wHour
           << ":"
           << std::setfill('0') << std::setw(2) << localTime.wMinute
           << ":"
           << std::setfill('0') << std::setw(2) << localTime.wSecond
           << "] ";

    m_currentMessageType = type;
    return m_stream;
}

void logger_t::out() {
    std::string message = m_date.str()
                          + c_loggerMessageHeader[m_currentMessageType]
                          + m_stream.str();

    std::cout << m_date.str();
    this->color(c_loggerMessageColor[m_currentMessageType]);
    std::cout << c_loggerMessageHeader[m_currentMessageType] << m_stream.str() << std::endl;
    this->color(7); // Reset color.

    if(m_file) {
        m_file << m_date.str() << c_loggerMessageHeader[m_currentMessageType] << m_stream.str() << std::endl;
    }

    m_stream.str("");
    m_date.str("");

    m_synchronizer.unlock();
}