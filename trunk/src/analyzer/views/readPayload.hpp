#pragma once

#include <analyzer/views/payload.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

class ReadPayload: public Payload
{
public:
    void insertNumericField(const std::string &value);
    void insertStringField(const std::string &value);
    void insertWideStringField(const std::string &value);
};

}
}
}
