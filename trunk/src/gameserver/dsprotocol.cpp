#include "dsprotocol.h"
#include "..\shared\enum.h"

dataServerProtocol_t::dataServerProtocol_t(dataServerProtocolExecutorInterface_t &iface,
											const sendCallback_t &sendCallback):
  m_executorInterface(iface),
  m_sendCallback(sendCallback) {}

void dataServerProtocol_t::core(const eMUCore::packet_t &packet) const {
	switch(packet.protocolId()) {
	case eMUShared::dataServerProtocol_e::_queryException:
		this->parseQueryExceptionNotice(packet);
		break;

	case eMUShared::dataServerProtocol_e::_accountManage:
		switch(packet.data()[3]) {
		case eMUShared::dataServerProtocol_e::accountManage_e::_check:
			this->parseAccountCheckAnswer(packet);
			break;
		}
		break;

	case eMUShared::dataServerProtocol_e::_characterManage:
		switch(packet.data()[3]) {
		case eMUShared::dataServerProtocol_e::characterManage_e::_list:
			this->parseCharacterListAnswer(packet);
			break;

		case eMUShared::dataServerProtocol_e::characterManage_e::_create:
			this->parseCharacterCreateAnswer(packet);
			break;

		case eMUShared::dataServerProtocol_e::characterManage_e::_delete:
			this->parseCharacterDeleteAnswer(packet);
			break;

		case eMUShared::dataServerProtocol_e::characterManage_e::_select:
			this->parseCharacterSelectAnswer(packet);
			break;
		}
		break;
	}
}

void dataServerProtocol_t::sendAccountCheckRequest(unsigned int connectionStamp,
														const std::string &accountId,
														const std::string &password,
														const std::string &ipAddress) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, eMUShared::dataServerProtocol_e::_accountManage);
	packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::accountManage_e::_check); // SubProtocolId
	packet.insert<unsigned int>(4, connectionStamp);
	packet.insertString(8, accountId, 10);
	packet.insertString(18, password, 10);
	packet.insertString(28, ipAddress, 16);

	m_sendCallback(packet);
}

void dataServerProtocol_t::parseAccountCheckAnswer(const eMUCore::packet_t &packet) const {
	unsigned int connectionStamp = packet.read<unsigned int>(4);
	std::string accountId = packet.readString(8, 10);
	accountCheckResult_e::type_t result = static_cast<accountCheckResult_e::type_t>(packet.read<unsigned char>(18));

	m_executorInterface.onAccountCheckAnswer(connectionStamp, accountId, result);
}

void dataServerProtocol_t::sendCharacterListRequest(unsigned int connectionStamp,
															const std::string &accountId) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
	packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_list); // SubProtocolId
	packet.insert<unsigned int>(4, connectionStamp);
	packet.insertString(8, accountId, 10);

	m_sendCallback(packet);
}

void dataServerProtocol_t::parseCharacterListAnswer(const eMUCore::packet_t &packet) const {
	unsigned int connectionStamp = packet.read<unsigned int>(4);
	size_t charactersCount = packet.read<unsigned char>(8);

	eMUShared::characterList_t characterList;

	for(size_t i = 0; i < charactersCount; ++i) {
		size_t step = i * 14;

		eMUShared::characterListAttributes_t character;
		character.m_name = packet.readString(9 + step, 10);
		character.m_race = packet.read<unsigned char>(19 + step);
		character.m_level = packet.read<unsigned short>(20 + step);
		character.m_controlCode = packet.read<unsigned char>(22 + step);

		characterList.push_back(character);
	}

	m_executorInterface.onCharacterListAnswer(connectionStamp, characterList);
}

void dataServerProtocol_t::sendLogoutRequest(const std::string &accountId) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, eMUShared::dataServerProtocol_e::_accountManage);
	packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::accountManage_e::_logout); // SubProtocolId.
	packet.insertString(4, accountId, 10);

	m_sendCallback(packet);
}

void dataServerProtocol_t::sendCharacterCreateRequest(unsigned int connectionStamp,
														const std::string &accountId,
														const std::string &name,
														eMUShared::characterRace_e::type_t race) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
	packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_create);
	packet.insert<unsigned int>(4, connectionStamp);
	packet.insertString(8, accountId, 10);
	packet.insertString(18, name, 10);
	packet.insert<unsigned char>(28, race);

	m_sendCallback(packet);
}

void dataServerProtocol_t::parseCharacterCreateAnswer(const eMUCore::packet_t &packet) const {
	unsigned int connectionStamp = packet.read<unsigned int>(4);
	std::string name = packet.readString(8, 10);
	eMUShared::characterRace_e::type_t race = static_cast<eMUShared::characterRace_e::type_t>(packet.read<unsigned char>(18));
	characterCreateResult_e::type_t result = static_cast<characterCreateResult_e::type_t>(packet.read<unsigned char>(19));

	m_executorInterface.onCharacterCreateAnswer(connectionStamp,
													name,
													race,
													result);
}

void dataServerProtocol_t::sendCharacterDeleteRequest(unsigned int connectionStamp,
														const std::string &accountId,
														const std::string &name,
														const std::string &pin) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
	packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_delete);
	packet.insert<unsigned int>(4, connectionStamp);
	packet.insertString(8, accountId, 10);
	packet.insertString(18, name, 10);
	packet.insertString(28, pin, 7);

	m_sendCallback(packet);
}

void dataServerProtocol_t::parseCharacterDeleteAnswer(const eMUCore::packet_t &packet) const {
	unsigned int connectionStamp = packet.read<unsigned int>(4);
	std::string name = packet.readString(8, 10);
	characterDeleteResult_e::type_t result = static_cast<characterDeleteResult_e::type_t>(packet.read<unsigned char>(18));

	m_executorInterface.onCharacterDeleteAnswer(connectionStamp, name, result);
}

void dataServerProtocol_t::sendCharacterSelectRequest(unsigned int connectionStamp,
														const std::string &accountId,
														const std::string &name) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
	packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_select);
	packet.insert<unsigned int>(4, connectionStamp);
	packet.insertString(8, accountId, 10);
	packet.insertString(18, name, 10);

	m_sendCallback(packet);
}

void dataServerProtocol_t::parseCharacterSelectAnswer(const eMUCore::packet_t &packet) const {
	unsigned int connectionStamp = packet.read<unsigned int>(4);

	eMUShared::characterAttributes_t attr;
	attr.m_name = packet.readString(8, 10);
	attr.m_race = static_cast<eMUShared::characterRace_e::type_t>(packet.read<unsigned char>(18));
	attr.m_position.set(packet.read<unsigned char>(19), packet.read<unsigned char>(20));
	attr.m_mapId = packet.read<unsigned char>(21);
	attr.m_direction = packet.read<unsigned char>(22);
	attr.m_experience = packet.read<unsigned int>(23);
	attr.m_levelUpPoints = packet.read<unsigned short>(27);
	attr.m_level = packet.read<unsigned short>(29);
	attr.m_strength = packet.read<unsigned short>(31);
	attr.m_agility = packet.read<unsigned short>(33);
	attr.m_vitality = packet.read<unsigned short>(35);
	attr.m_energy = packet.read<unsigned short>(37);
	attr.m_health = packet.read<unsigned short>(39);
	attr.m_maxHealth = packet.read<unsigned short>(41);
	attr.m_mana = packet.read<unsigned short>(43);
	attr.m_maxMana = packet.read<unsigned short>(45);
	attr.m_shield = packet.read<unsigned short>(47);
	attr.m_maxShield = packet.read<unsigned short>(49);
	attr.m_stamina = packet.read<unsigned short>(51);
	attr.m_maxStamina = packet.read<unsigned short>(53);
	attr.m_money = packet.read<unsigned int>(55);
	attr.m_pkLevel = packet.read<unsigned char>(59);
	attr.m_controlCode = packet.read<unsigned char>(60);
	attr.m_addPoints = packet.read<unsigned short>(61);
	attr.m_maxAddPoints = packet.read<unsigned short>(63);
	attr.m_command = packet.read<unsigned short>(65);
	attr.m_minusPoints = packet.read<unsigned short>(67);
	attr.m_maxMinusPoints = packet.read<unsigned short>(69);

	m_executorInterface.onCharacterSelectAnswer(connectionStamp, attr);
}

void dataServerProtocol_t::sendCharacterSaveRequest(const std::string &accountId,
													const eMUShared::characterAttributes_t &attr) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
	packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_save);
	packet.insertString(4, accountId, 10);
	packet.insertString(14, attr.m_name, 10);
	packet.insert<unsigned char>(24, attr.m_race);
	packet.insert<unsigned char>(25, attr.m_position.m_x);
	packet.insert<unsigned char>(26, attr.m_position.m_y);
	packet.insert<unsigned char>(27, attr.m_mapId);
	packet.insert<unsigned char>(28, attr.m_direction);
	packet.insert<unsigned int>(29, attr.m_experience);
	packet.insert<unsigned short>(33, attr.m_levelUpPoints);
	packet.insert<unsigned short>(35, attr.m_level);
	packet.insert<unsigned short>(37, attr.m_strength);
	packet.insert<unsigned short>(39, attr.m_agility);
	packet.insert<unsigned short>(41, attr.m_vitality);
	packet.insert<unsigned short>(43, attr.m_energy);
	packet.insert<unsigned short>(45, attr.m_health);
	packet.insert<unsigned short>(47, attr.m_maxHealth);
	packet.insert<unsigned short>(49, attr.m_mana);
	packet.insert<unsigned short>(51, attr.m_maxMana);
	packet.insert<unsigned short>(53, attr.m_shield);
	packet.insert<unsigned short>(55, attr.m_maxShield);
	packet.insert<unsigned short>(57, attr.m_stamina);
	packet.insert<unsigned short>(59, attr.m_maxStamina);
	packet.insert<unsigned int>(61, attr.m_money);
	packet.insert<unsigned char>(65, attr.m_pkLevel);
	packet.insert<unsigned char>(66, attr.m_controlCode);
	packet.insert<unsigned short>(67, attr.m_addPoints);
	packet.insert<unsigned short>(69, attr.m_maxAddPoints);
	packet.insert<unsigned short>(71, attr.m_command);
	packet.insert<unsigned short>(73, attr.m_minusPoints);
	packet.insert<unsigned short>(75, attr.m_maxMinusPoints);

	m_sendCallback(packet);
}

void dataServerProtocol_t::parseQueryExceptionNotice(const eMUCore::packet_t &packet) const {
	unsigned int connectionStamp = packet.read<unsigned int>(4);
	std::string what = packet.readString(8, 512);

	m_executorInterface.onQueryExceptionNotice(connectionStamp, what);
}