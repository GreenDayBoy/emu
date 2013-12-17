#pragma once

#include <analyzer/stream/views/streamView.hpp>

namespace eMU
{
namespace analyzer
{
namespace stream
{
namespace views
{

class ReadStreamView: public StreamView
{
public:
    void insertNumericField(const std::string &value);
    void insertStringField(const std::string &value);
};

}
}
}
}
