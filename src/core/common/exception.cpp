#include <common/exception.hpp>

namespace eMU {
namespace core {
namespace common {

Exception::Exception() {}

Exception::Exception(const Exception &exception) {
    stream_ << exception.stream_.rdbuf();
}

std::stringstream& Exception::in() {
    stream_.str("");

    return stream_;
}

std::string Exception::what() const {
    return stream_.str();
}

}
}
}
