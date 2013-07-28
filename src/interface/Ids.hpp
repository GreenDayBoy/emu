#pragma once

namespace eMU
{
namespace interface
{

namespace ProtocolId
{

enum Id
{
    CONNECT_SERVER_PROTOCOL = 0xF4,
};

}

namespace MessageId
{

enum Id
{
    GAME_SERVER_ADDRESS_RESPONSE = 0x03,
    GAME_SERVERS_LIST_RESPONSE = 0x06,
};

}

}
}
