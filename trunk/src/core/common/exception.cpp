#include <common/exception.hpp>

namespace eMU {
namespace core {

exception_t::exception_t() {}

exception_t::exception_t(const std::string &message):
  m_message(message) {}

exception_t::exception_t(const exception_t &e) {
    m_stream << e.m_stream.rdbuf();
}

std::stringstream& exception_t::in() {
    m_stream.str("");
    return m_stream;
}

const char* exception_t::what() {
    m_message = m_stream.str();
    return m_message.c_str();
}

}
}