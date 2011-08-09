#ifndef eMU_CORE_ENUM_H
#define eMU_CORE_ENUM_H

namespace eMUCore {
namespace loggerMessage_e {
enum type_t {
    _debug,
    _info,
    _warning,
    _error,
    _protocol
};
};

namespace ioBuffer_e {
enum type_t {
    _recv = 0x500,
    _send
};
};

namespace schedule_e {
enum type_t {
    _synchronized = 0x37A,
    _nonSynchronized
};
};
};

#endif // eMU_CORE_ENUM_H