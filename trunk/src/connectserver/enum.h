#ifndef eMU_CONNECTSERVER_ENUM_H
#define eMU_CONNECTSERVER_ENUM_H

namespace protocol_e {
enum type_t {
    _handshake = 0x00,
    _serverListManage = 0xF4,
};

namespace serverListManage_e {
enum type_t {
    _serverSelect = 0x03,
    _get = 0x06
};
};
};

#endif // eMU_CONNECTSERVER_ENUM_H