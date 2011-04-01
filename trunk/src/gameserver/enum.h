#ifndef eMU_GAMESERVER_ENUM_H
#define eMU_GAMESERVER_ENUM_H

namespace cryptKeys_e {
	enum type_t {
		_encrypt = 0,
		_decrypt
	};
};

namespace gameObject_e {
	enum type_t {
		_character = 0xE0,
		_monster
	};
};

namespace mapTileAttribute_e {
	enum type_t {
		_ground = 0,
		_safezone,
		_safeZoneNoneGround,
		_nonGround
	};
};

namespace protocol_e {
	enum type_t {
		_publicChat = 0x00,
		_textNotice = 0x0D,
		_characterViewportCreate = 0x12,
		_monsterViewportCreate = 0x013,
		_viewportDestroy = 0x014,
		_action = 0x18,
		_accountManage = 0xF1,
		_characterManage = 0xF3,
		_move = 0xD7,
		_teleport = 0x1C
	};

	namespace accountManage_e {
		enum type_t {
			_handshake = 0x00,
			_login = 0x01,
			_logout = 0x02
		};
	};

	namespace characterManage_e {
		enum type_t {
			_list = 0x00,
			_create = 0x01,
			_delete = 0x02,
			_select = 0x03
		};
	};
};

namespace characterAction_e {
	enum type_t {
		_setStand = 0x7B,
		_setSit = 0x80,
		_setLeaning = 0x81,
		_setLevitation = 0x82
	};
};

namespace characterPose_e {
	enum type_t {
		_stand = 0,
		_sit = 2,
		_leaning = 3,
		_levitation = 4
	};
};

namespace clientCloseReason_e {
	enum type_t {
		_exit = 0x00,
		_switchCharacter = 0x01,
		_selectServer = 0x02,
		_none = 0xFF

	};
};

namespace availableRace_e {
	enum type_t {
		_basic = 0x02,
		_magicGladiator = 0x03,
		_darkLord = 0x04
	};
};

namespace accountCheckResult_e {
	enum type_t {
		_invalidPassword = 0x00,
		_success = 0x01,
		_notExists = 0x02,
		_alreadyInUse = 0x03,
		_banned = 0x05,
		_invalidVersionOrSerial = 0x06,
		_checkAttemptsExceeded = 0x08
	};
};

namespace gameNotice_e {
	enum type_t {
		_gold = 0x00,
		_blue = 0x01
	};
};

namespace characterCreateResult_e {
	enum type_t {
		_alreadyExists = 0x00,
		_success = 0x01,
		_toManyCharacters = 0x02
	};
};

namespace characterDeleteResult_e {
	enum type_t {
		_noAccountAssociation = 0x00,
		_success = 0x01,
		_invalidPin = 0x02
	};
};

#endif // eMU_GAMESERVER_ENUM_H