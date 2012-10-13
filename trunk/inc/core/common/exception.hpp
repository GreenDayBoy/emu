#pragma once

#include <sstream>
#include <string>

#include <core.hpp>

namespace eMU {
namespace core {

class eMU_CORE_DECLSPEC exception_t: public std::exception {
public:
    exception_t();
    exception_t(const std::string &message);
    exception_t(const exception_t &e);
    virtual ~exception_t() throw() {}

    std::stringstream& in();
    const char* what();

private:
    std::stringstream m_stream;
    std::string m_message;
};

}
}
