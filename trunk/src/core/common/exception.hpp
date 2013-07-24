#pragma once

#include <sstream>
#include <string>

namespace eMU {
namespace core {
namespace common {

class Exception {
public:
    Exception();
    explicit Exception(const std::string &message);
    Exception(const Exception &exception);

    std::stringstream& in();
    std::string what() const;

private:
    std::stringstream stream_;
};

}
}
}
