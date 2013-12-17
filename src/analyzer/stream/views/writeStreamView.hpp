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

class WriteStreamView: public StreamView
{
public:
    void resize(size_t fieldsCount);
    const fields::FieldsContainer& getFields() const;

private:
    void insertFields(size_t fieldsCount);
    void removeFields(size_t fieldsCount);
};

}
}
}
}
