#include "protocol.h"
#include "monster.h"

protocol_t::protocol_t(protocolExecutorInterface_t &iface,
						const sendCallback_t &sendCallback):
  m_executorInterface(iface),
  m_sendCallback(sendCallback) {}

void protocol_t::core(gameServerUser_t &user,
						const eMUCore::packet_t &packet) const {
	switch(packet.getProtocolId()) {
	case 0x00:
		this->parsePublicChatRequest(user, packet);
		break;

	case 0x18:
		this->parseCharacterActionRequest(user, packet);
		break;

	case 0xF1:
		switch(packet.getData()[3]) {
		case 0x01:
			this->parseLoginRequest(user, packet);
			break;
		case 0x02:
			this->parseLogoutRequest(user, packet);
			break;
		}
		break;

	case 0xF3:
		switch(packet.getData()[3]) {
		case 0x00:
			this->parseCharacterListRequest(user, packet);
			break;

		case 0x01:
			this->parseCharacterCreateRequest(user, packet);
			break;

		case 0x02:
			this->parseCharacterDeleteRequest(user, packet);
			break;

		case 0x03:
			this->parseCharacterSelectRequest(user, packet);
			break;
		}
		break;

	case 0xD7:
		this->parseCharacterMoveRequest(user, packet);
		break;

	case 0x1C:
		this->parseCharacterTeleportRequest(user, packet);
		break;
	}
}

void protocol_t::sendHandshake(gameServerUser_t &user, unsigned short userIndex, std::string version) const {
	//[0] HeaderId: 0xC1
	//[1] Size
	//[2] ProtocolId: 0xF1
	//[3] SubProtocolId: 0x00
	//[4] Result
	//[5] Index
	//[6] Index
	//[7] Version without '.'
	// ------------------

	eMUCore::packet_t packet;
	packet.construct(0xC1, 0xF1);
	packet.insert<unsigned char>(3, 0x00);
	packet.insert<unsigned char>(4, 0x01);
	packet.insert<unsigned short>(5, _byteswap_ushort(userIndex));
	packet.insertString(7, version, 5);

	m_sendCallback(user, packet);
}

void protocol_t::parseLoginRequest(gameServerUser_t &user, 
								   const eMUCore::packet_t &packet) const {
	std::string accountId = this->xorString(packet.readString(4, 10));
	std::string password = this->xorString(packet.readString(14, 10));
	std::string clientExeVersion = packet.readString(28, 5);
	std::string clientExeSerial = packet.readString(33, 16);

	m_executorInterface.onLoginRequest(user,
										accountId,
										password,
										clientExeVersion,
										clientExeSerial);
}

void protocol_t::sendLoginAnswer(gameServerUser_t &user, 
											unsigned char result) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0xF1);
	packet.insert<unsigned char>(3, 0x01);
	packet.insert<unsigned char>(4, result);

	m_sendCallback(user, packet);
}

void protocol_t::parseCharacterListRequest(gameServerUser_t &user, 
											const eMUCore::packet_t &packet) const {
	m_executorInterface.onCharacterListRequest(user);
}

void protocol_t::sendCharacterListAnswer(gameServerUser_t &user,
											   unsigned char availableRaces,
											   const eMUShared::characterList_t &characterList) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0xF3);
	packet.insert<unsigned char>(3, 0x00); // SubProtocolId.
	packet.insert<unsigned char>(4, availableRaces);
	packet.insert<unsigned char>(5, 0); // ???
	packet.insert<unsigned char>(6, characterList.size());
	

	for(size_t i = 0; i < characterList.size(); ++i) {
		size_t step = i * 34;

		packet.insert<unsigned char>(7 + step, i);
		packet.insertString(8 + step, characterList[i].m_name, 10);
		packet.insert<unsigned char>(18 + step, 0x00); // ???
		packet.insert<unsigned short>(19 + step, characterList[i].m_level);
		packet.insert<unsigned char>(21 + step, characterList[i].m_controlCode);

		unsigned char protoRace = ((characterList[i].m_race >> 4) << 5) & 0xE0;
		protoRace |= ((characterList[i].m_race & 0x07) << 4) & 0x10;
		packet.insert<unsigned char>(22 + step, protoRace);
		packet.insert<unsigned char>(23 + step, 0xFF);
		packet.insert<unsigned char>(24 + step, 0xFF);
		packet.insert<unsigned char>(25 + step, 0xFF);
		packet.insert<unsigned char>(26 + step, 0xFF);
		packet.insert<unsigned char>(27 + step, 0xFF);
		packet.insert<unsigned char>(28 + step, 0x00);
		packet.insert<unsigned char>(29 + step, 0x00);
		packet.insert<unsigned char>(30 + step, 0x00);
		packet.insert<unsigned char>(31 + step, 0xF8);
		packet.insert<unsigned char>(32 + step, 0x00);
		packet.insert<unsigned char>(33 + step, 0x00);
		packet.insert<unsigned char>(34 + step, 0xF0);
		packet.insert<unsigned char>(35 + step, 0xFF);
		packet.insert<unsigned char>(36 + step, 0xFF);
		packet.insert<unsigned char>(37 + step, 0xFF);
		packet.insert<unsigned char>(38 + step, 0x00);
		packet.insert<unsigned char>(39 + step, 0x00);
		packet.insert<unsigned char>(40 + step, 0xFF); // guild status.
	}

	m_sendCallback(user, packet);
}

void protocol_t::parseLogoutRequest(gameServerUser_t &user,
									const eMUCore::packet_t &packet) const {
	unsigned char closeReason = packet.read<unsigned char>(4);
	m_executorInterface.onLogoutRequest(user, closeReason);
}

void protocol_t::sendLogoutAnswer(gameServerUser_t &user,
									   unsigned char closeReason) const {
	eMUCore::packet_t packet;
	packet.construct(0xC3, 0xF1);
	packet.insert<unsigned char>(3, 0x02); // SubProtocolId.
	packet.insert<unsigned char>(4, closeReason);

	m_sendCallback(user, packet);
}

void protocol_t::parseCharacterCreateRequest(gameServerUser_t &user,
												const eMUCore::packet_t &packet) const {
	std::string name = packet.readString(4, 10);
	unsigned char race = packet.read<unsigned char>(14);

	m_executorInterface.onCharacterCreateRequest(user, name, race);
}

void protocol_t::sendCharacterCreteAnswer(gameServerUser_t &user,
												unsigned char result,
												const std::string &name,
												int slot,
												unsigned short level,
												unsigned char race) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0xF3);
	packet.insert<unsigned char>(3, 0x01);
	packet.insert<unsigned char>(4, result);
	packet.insertString(5, name, 10);
	packet.insert<unsigned char>(15, slot);
	packet.insert<unsigned short>(16, level);
	packet.insert<unsigned char>(18, race << 1); // I don't know for what is left shifting ;/.

	// temporary :). Inventory here.
	for(size_t i = 0; i < 24; ++i) {
		packet.insert<unsigned char>(19 + i, 0x00);
	}

	m_sendCallback(user, packet);
}

void protocol_t::parseCharacterDeleteRequest(gameServerUser_t &user,
											 const eMUCore::packet_t &packet) const {
	std::string name = packet.readString(4, 10);
	std::string pin = packet.readString(14, 7);

	m_executorInterface.onCharacterDeleteRequest(user, name, pin);
}

void protocol_t::sendCharacterDeleteAnswer(gameServerUser_t &user,
												unsigned char result) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0xF3);
	packet.insert<unsigned char>(3, 0x02);
	packet.insert<unsigned char>(4, result);

	m_sendCallback(user, packet);
}

void protocol_t::parseCharacterSelectRequest(gameServerUser_t &user,
											 const eMUCore::packet_t &packet) const {
	std::string name = packet.readString(4, 10);
	m_executorInterface.onCharacterSelectRequest(user, name);
}

void protocol_t::sendCharacterSelectAnswer(gameServerUser_t &user,
												const character_t &character) const {
	eMUCore::packet_t packet;
	packet.construct(0xC3, 0xF3);
	packet.insert<unsigned char>(3, 0x03);
	packet.insert<unsigned char>(4, character.getPosX());
	packet.insert<unsigned char>(5, character.getPosY());
	packet.insert<unsigned char>(6, character.getMapId());
	packet.insert<unsigned char>(7, character.getDirection());
	packet.insert<unsigned int>(8, character.getExperience());
	packet.insert<unsigned int>(12, character.getNextExperience());
	packet.insert<unsigned short>(16, character.getLevelUpPoints());
	packet.insert<unsigned short>(18, character.getStrength());
	packet.insert<unsigned short>(20, character.getAgility());
	packet.insert<unsigned short>(22, character.getVitality());
	packet.insert<unsigned short>(24, character.getEnergy());
	packet.insert<unsigned short>(26, character.getHealth());
	packet.insert<unsigned short>(28, character.getMaxHealth());
	packet.insert<unsigned short>(30, character.getMana());
	packet.insert<unsigned short>(32, character.getMaxMana());
	packet.insert<unsigned short>(34, character.getShield());
	packet.insert<unsigned short>(36, character.getMaxShield());
	packet.insert<unsigned short>(38, character.getStamina());
	packet.insert<unsigned short>(40, character.getMaxStamina());
	packet.insert<unsigned int>(42, character.getMoney());
	packet.insert<unsigned char>(46, character.getPkLevel());
	packet.insert<unsigned char>(47, character.getControlCode());
	packet.insert<unsigned short>(48, character.getAddPoints());
	packet.insert<unsigned short>(50, character.getMaxAddPoints());
	packet.insert<unsigned short>(52, character.getCommand());
	packet.insert<unsigned short>(54, character.getMinusPoints());
	packet.insert<unsigned short>(56, character.getMaxMinusPoints());

	m_sendCallback(user, packet);
}

void protocol_t::sendTextNotice(gameServerUser_t &user,
										unsigned char type,
										const std::string &notice,
										unsigned char loopCount,
										unsigned short loopDelay,
										unsigned int color,
										unsigned char speed) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0x0D);
	packet.insert<unsigned char>(3, type);
	packet.insert<unsigned char>(4, loopCount);
	packet.insert<unsigned short>(5, loopDelay);
	packet.insert<unsigned char>(7, 0); // unk.
	packet.insert<unsigned int>(8, color);
	packet.insert<unsigned char>(12, speed);

	size_t noticeLen = std::min<size_t>(notice.size(), 241);

	packet.insertString(13, notice, noticeLen); // 241 - max notice size.
	packet.insert<unsigned char>(13 + noticeLen, 0);	// cause webzen client want null terminated string
													// I don't know why if login, password or name haven't to be null terminated. 

	m_sendCallback(user, packet);
}

void protocol_t::parseCharacterMoveRequest(gameServerUser_t &user,
										   const eMUCore::packet_t &packet) const {
	int stepsCount = packet.read<unsigned char>(5) & 0x0F;

	if(stepsCount <= 15) {
		if(stepsCount > 0) {
			++stepsCount;
		}

		unsigned char x = packet.read<unsigned char>(3);
		unsigned char y = packet.read<unsigned char>(4);
		unsigned char direction = packet.read<unsigned char>(5) >> 4;
		map_t::path_t path;

		char directions[] = {-1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0};
		const unsigned char *data = &packet.getData()[5];

		for(int i = 1; i < stepsCount; ++i) {
			int directionId = 0;

			if((i & 1) == 1) {
				directionId = data[(i + 1) >> 1] >> 4;
			} else {
				directionId = data[(i + 1) >> 1] & 0x0F;
			}

			x += directions[directionId << 1];
			y += directions[(directionId << 1) + 1];

			path.push_back(map_t::position_t(x, y));
		}

		m_executorInterface.onCharacterMoveRequest(user, x, y, direction, path);
	}
}

void protocol_t::parseCharacterTeleportRequest(gameServerUser_t &user,
											   const eMUCore::packet_t &packet) const {
	unsigned short gateId = packet.read<unsigned short>(3);
	m_executorInterface.onCharacterTeleportRequest(user, gateId);
}

void protocol_t::sendCharacterTeleportAnswer(gameServerUser_t &user,
													unsigned char mapId,
													unsigned char x,
													unsigned char y,
													unsigned char direction,
													unsigned char gateId) const {
	eMUCore::packet_t packet;
	packet.construct(0xC3, 0x1C);
	packet.insert<unsigned char>(3, gateId);
	packet.insert<unsigned char>(4, mapId);
	packet.insert<unsigned char>(5, x);
	packet.insert<unsigned char>(6, y);
	packet.insert<unsigned char>(7, direction);

	m_sendCallback(user, packet);
}

void protocol_t::sendViewportCharacterCreateRequest(gameServerUser_t &user,
														 viewportManager_t::viewport_t &viewport) const {
	eMUCore::packet_t packet;
	packet.construct(0xC2, 0x12);

	size_t count = 0;

	for(viewportManager_t::viewport_t::iterator i = viewport.begin(); i != viewport.end(); ++i) {
		gameObject_t *object = (*i);

		if(object->getType() == gameObject_t::_OBJECT_CHARACTER) {
			character_t *character = reinterpret_cast<character_t*>(object);

			unsigned short step = count * 41;

			if(character->isTeleportEffect()) {
				packet.insert<unsigned char>(5 + step, HIBYTE(character->getOwner().getIndex()) | 0x80);
			} else {
				packet.insert<unsigned char>(5 + step, HIBYTE(character->getOwner().getIndex()));
			}
			packet.insert<unsigned char>(6 + step, LOBYTE(character->getOwner().getIndex()));
			packet.insert<unsigned char>(7 + step, character->getPosX());
			packet.insert<unsigned char>(8 + step, character->getPosY());

			packet.insert<unsigned char>(9 + step, character->getPreview()[0]);
			packet.insert<unsigned char>(10 + step, character->getPreview()[1]);
			packet.insert<unsigned char>(11 + step, character->getPreview()[2]);
			packet.insert<unsigned char>(12 + step, character->getPreview()[3]);
			packet.insert<unsigned char>(13 + step, character->getPreview()[4]);
			packet.insert<unsigned char>(14 + step, character->getPreview()[5]);
			packet.insert<unsigned char>(15 + step, character->getPreview()[6]);
			packet.insert<unsigned char>(16 + step, character->getPreview()[7]);
			packet.insert<unsigned char>(17 + step, character->getPreview()[8]);
			packet.insert<unsigned char>(18 + step, character->getPreview()[9]);
			packet.insert<unsigned char>(19 + step, character->getPreview()[10]);
			packet.insert<unsigned char>(20 + step, character->getPreview()[11]);
			packet.insert<unsigned char>(21 + step, character->getPreview()[12]);
			packet.insert<unsigned char>(22 + step, character->getPreview()[13]);
			packet.insert<unsigned char>(23 + step, character->getPreview()[14]);
			packet.insert<unsigned char>(24 + step, character->getPreview()[15]);
			packet.insert<unsigned char>(25 + step, character->getPreview()[16]);
			packet.insert<unsigned char>(26 + step, character->getPreview()[17]);

			packet.insert<unsigned short>(27 + step, 0); // dummy - Dear WebZen forgot about structure alignment.
			packet.insert<unsigned int>(29 + step, 0);

			packet.insertString(33 + step, character->getName(), 10);
			packet.insert<unsigned char>(43 + step, character->getPosX());
			packet.insert<unsigned char>(44 + step, character->getPosY());
			packet.insert<unsigned char>(45 + step, character->getDirection() << 4);
			++count;
		}
	}

	packet.insert<unsigned char>(4, static_cast<unsigned char>(count));

	if(count > 0) {
		m_sendCallback(user, packet);
	}
}

void protocol_t::sendViewportMonsterCreateRequest(gameServerUser_t &user,
													   viewportManager_t::viewport_t &viewport) const {
	eMUCore::packet_t packet;
	packet.construct(0xC2, 0x13);

	size_t count = 0;

	for(viewportManager_t::viewport_t::iterator i = viewport.begin(); i != viewport.end(); ++i) {
		gameObject_t *object = (*i);

		if(object->getType() == gameObject_t::_OBJECT_MONSTER) {
			monster_t *monster = reinterpret_cast<monster_t*>(object);

			unsigned short step = count * 13;

			packet.insert<unsigned char>(5 + step, HIBYTE(monster->getIndex()));
			packet.insert<unsigned char>(6 + step, LOBYTE(monster->getIndex()));
			packet.insert<unsigned char>(7 + step, HIBYTE(monster->getId()));
			packet.insert<unsigned char>(8 + step, LOBYTE(monster->getId()));
			packet.insert<unsigned int>(9 + step, 0);
			packet.insert<unsigned char>(13 + step, monster->getPosX());
			packet.insert<unsigned char>(14 + step, monster->getPosY());
			packet.insert<unsigned char>(15 + step, monster->getPosX());
			packet.insert<unsigned char>(16 + step, monster->getPosY());
			packet.insert<unsigned char>(17 + step, monster->getDirection() << 4);

			++count;
		}
	}

	packet.insert<unsigned char>(4, static_cast<unsigned char>(count));

	if(count > 0) {
		m_sendCallback(user, packet);
	}
}

void protocol_t::sendViewportDestroyRequest(gameServerUser_t &user,
												 viewportManager_t::viewport_t &viewport) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0x14);

	size_t count = 0;

	for(viewportManager_t::viewport_t::iterator i = viewport.begin(); i != viewport.end(); ++i) {
		unsigned short index = 0;
		gameObject_t *object = (*i);

		if(object->getType() == gameObject_t::_OBJECT_CHARACTER) {
			index = reinterpret_cast<character_t*>(object)->getOwner().getIndex();
		} else {
			index = reinterpret_cast<monster_t*>(object)->getIndex();
		}

		unsigned short step = count * 2;
		packet.insert<unsigned char>(4 + step, HIBYTE(index));
		packet.insert<unsigned char>(5 + step, LOBYTE(index));

		++count;
	}

	packet.insert<unsigned char>(3, static_cast<unsigned char>(count));

	if(count > 0) {
		m_sendCallback(user, packet);
	}
}

void protocol_t::sendViewportObjectMoveRequest(gameObject_t &object) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0xD7);

	unsigned short index = 0;

	if(object.getType() == gameObject_t::_OBJECT_CHARACTER) {
		index = reinterpret_cast<character_t&>(object).getOwner().getIndex();
	} else {
		index = reinterpret_cast<monster_t&>(object).getIndex();
	}

	packet.insert<unsigned char>(3, HIBYTE(index));
	packet.insert<unsigned char>(4, LOBYTE(index));
	packet.insert<unsigned char>(5, object.getPosX());
	packet.insert<unsigned char>(6, object.getPosY());
	packet.insert<unsigned char>(7, object.getDirection() << 4);

	for(viewportManager_t::viewport_t::iterator i = object.getViewport().begin(); i != object.getViewport().end(); ++i) {
		gameObject_t *vo = (*i);

		if(vo->getType() == gameObject_t::_OBJECT_CHARACTER) {
			m_sendCallback(reinterpret_cast<character_t*>(vo)->getOwner(), packet);
		}
	}
}

void protocol_t::parsePublicChatRequest(gameServerUser_t &user,
										const eMUCore::packet_t &packet) const {
	std::string message = packet.readString(13, 59);
	m_executorInterface.onPublicChat(user, message);
}

void protocol_t::sendPublicChatAnswer(gameServerUser_t &user, const std::string &message) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0x00);
	packet.insertString(3, user.getCharacter().getName(), 10);

	size_t messageLen = std::min<size_t>(message.size(), 59);
	packet.insertString(13, message, messageLen);
	packet.insert<unsigned char>(13 + messageLen, 0);

	for(viewportManager_t::viewport_t::iterator i = user.getCharacter().getViewport().begin(); i != user.getCharacter().getViewport().end(); ++i) {
		gameObject_t *vo = (*i);

		if(vo->getType() == gameObject_t::_OBJECT_CHARACTER) {
			m_sendCallback(reinterpret_cast<character_t*>(vo)->getOwner(), packet);
		}
	}

	m_sendCallback(user, packet);
}

void protocol_t::parseCharacterActionRequest(gameServerUser_t &user,
											 const eMUCore::packet_t &packet) const {
	unsigned char direction = packet.read<unsigned char>(3);
	unsigned char actionId = packet.read<unsigned char>(4);

	m_executorInterface.onCharacterAction(user, direction, actionId);
}

void protocol_t::sendViewportCharacterActionRequest(character_t &character,
													unsigned char actionId,
													unsigned short targetId) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, 0x18);
	packet.insert<unsigned char>(3, HIBYTE(character.getOwner().getIndex()));
	packet.insert<unsigned char>(4, LOBYTE(character.getOwner().getIndex()));
	packet.insert<unsigned char>(5, character.getDirection());
	packet.insert<unsigned char>(6, actionId);
	packet.insert<unsigned char>(7, HIBYTE(targetId));
	packet.insert<unsigned char>(8, LOBYTE(targetId));

	for(viewportManager_t::viewport_t::iterator i = character.getViewport().begin(); i != character.getViewport().end(); ++i) {
		gameObject_t *vo = (*i);

		if(vo->getType() == gameObject_t::_OBJECT_CHARACTER) {
			m_sendCallback(reinterpret_cast<character_t*>(vo)->getOwner(), packet);
		}
	}
}

std::string protocol_t::xorString(const std::string &buff) const {
	unsigned char xorLoginKeys[3] = {0xFC, 0xCF, 0xAB};

	std::string encrypted;

	for(size_t i = 0; i < 10; ++i) {
		char value = buff[i] ^ xorLoginKeys[i % 3];

		if(value != 0) {
			encrypted.push_back(value);
		}
	}

	return encrypted;
}