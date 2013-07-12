#pragma once

#include <sstream>
#include <string>

#include <core.hpp>

namespace eMU {
namespace core {
namespace common {

class Exception {
public:
    Exception();
    Exception(const Exception &exception);

    std::stringstream& in();
    std::string what() const;

private:
    std::stringstream stream_;
};

}
}
}
