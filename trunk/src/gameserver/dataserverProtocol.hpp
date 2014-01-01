#pragma once

#include <protocols/client.hpp>
#include <gameserver/context.hpp>

namespace eMU
{
namespace gameserver
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
