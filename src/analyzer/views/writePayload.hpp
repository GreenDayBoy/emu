#pragma once

#include <analyzer/views/payload.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

class WritePayload: public Payload
{
public:
    void resize(size_t fieldsCount);
    const FieldsContainer& getFields() const;
    std::string getDump() const;
    size_t getFieldsSize() const;

private:
    void insertFields(size_t fieldsCount);
    void removeFields(size_t fieldsCount);
};

}
}
}
