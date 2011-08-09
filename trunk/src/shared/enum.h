#ifndef eMU_SHARED_ENUM_H
#define eMU_SHARED_ENUM_H

#include <string>
#include <vector>

namespace eMUShared {
namespace dataServerProtocol_e {
enum type_t {
    _queryException = 0x00,
    _accountManage = 0xF1,
    _characterManage = 0xF3,
};

namespace accountManage_e {
enum type_t {
    _check = 0x01,
    _logout = 0x02
};
};

namespace characterManage_e {
enum type_t {
    _list = 0x00,
    _create = 0x01,
    _delete = 0x02,
    _select = 0x03,
    _save = 0x04
};
};
};

namespace characterRace_e {
enum type_t {
    _darkWizard = 0,
    _darkKnight = 16,
    _elf = 32,
    _magicGladiator = 48,
    _darkLord = 64,
    _none = 0xFF
};
};
};

#endif // eMU_SHARED_ENUM_H