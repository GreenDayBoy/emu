#pragma once

#include <protocols/client.hpp>
#include <loginserver/context.hpp>

namespace eMU
{
namespace loginserver
{

class DataserverProtocol: public protocols::Client
{
public:
    DataserverProtocol(Context &context);

private:
    bool handleReadStream(const streaming::ReadStream &stream);

    Context &context_;
};

}
}
