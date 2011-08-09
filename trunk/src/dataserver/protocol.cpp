#include "protocol.h"
#include "..\shared\enum.h"

protocol_t::protocol_t(const sendCallback_t &sendCallback):
    m_executorInterface(NULL),
    m_sendCallback(sendCallback) {}

void protocol_t::core(dataServerUser_t *user,
                      const eMUCore::packet_t &packet) const {
    switch(packet.protocolId()) {
    case eMUShared::dataServerProtocol_e::_accountManage:
        switch(packet.data()[3]) {
        case eMUShared::dataServerProtocol_e::accountManage_e::_check:
            this->parseAccountCheckRequest(user, packet);
            break;
        case eMUShared::dataServerProtocol_e::accountManage_e::_logout:
            this->parseLogoutRequest(user, packet);
            break;
        }
        break;

    case eMUShared::dataServerProtocol_e::_characterManage:
        switch(packet.data()[3]) {
        case eMUShared::dataServerProtocol_e::characterManage_e::_list:
            this->parseCharacterListRequest(user, packet);
            break;

        case eMUShared::dataServerProtocol_e::characterManage_e::_create:
            this->parseCharacterCreateRequest(user, packet);
            break;

        case eMUShared::dataServerProtocol_e::characterManage_e::_delete:
            this->parseCharacterDeleteRequest(user, packet);
            break;

        case eMUShared::dataServerProtocol_e::characterManage_e::_select:
            this->parseCharacterSelectRequest(user, packet);
            break;

        case eMUShared::dataServerProtocol_e::characterManage_e::_save:
            this->parseCharacterSaveRequest(user, packet);
            break;
        }
        break;
    }
}

void protocol_t::parseAccountCheckRequest(dataServerUser_t *user,
        const eMUCore::packet_t &packet) const {
    unsigned int connectionStamp = packet.read<unsigned int>(4);
    std::string accountId = packet.readString(8, 10);
    std::string password = packet.readString(18, 10);
    std::string ipAddress = packet.readString(28, 16);

    m_executorInterface->onAccountCheckRequest(user, connectionStamp, accountId, password, ipAddress);
}

void protocol_t::sendAccountCheckAnswer(dataServerUser_t *user,
                                        unsigned int connectionStamp,
                                        const std::string &accountId,
                                        unsigned char result) const {
    eMUCore::packet_t packet;
    packet.construct(0xC1, eMUShared::dataServerProtocol_e::_accountManage);
    packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::accountManage_e::_check);
    packet.insert<unsigned int>(4, connectionStamp);
    packet.insertString(8, accountId, 10);
    packet.insert<unsigned char>(18, result);

    m_sendCallback(user, packet);
}

void protocol_t::parseCharacterListRequest(dataServerUser_t *user,
        const eMUCore::packet_t &packet) const {
    unsigned int connectionStamp = packet.read<unsigned int>(4);
    std::string accountId = packet.readString(8, 10);

    m_executorInterface->onCharacterListRequest(user, connectionStamp, accountId);
}

void protocol_t::sendCharacterListAnswer(dataServerUser_t *user,
        unsigned int connectionStamp,
        const eMUShared::characterList_t &characterList) const {
    eMUCore::packet_t packet;
    packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
    packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_list);
    packet.insert<unsigned int>(4, connectionStamp);
    packet.insert<unsigned char>(8, characterList.size());

    for(size_t i = 0; i < characterList.size(); ++i) {
        size_t step = i * 14;

        packet.insertString(9 + step, characterList[i].m_name, 10);
        packet.insert<unsigned char>(19 + step, characterList[i].m_race);
        packet.insert<unsigned short>(20 + step, characterList[i].m_level);
        packet.insert<unsigned char>(22 + step, characterList[i].m_controlCode);
    }

    m_sendCallback(user, packet);
}

void protocol_t::parseLogoutRequest(dataServerUser_t *user,
                                    const eMUCore::packet_t &packet) const {
    std::string accountId = packet.readString(4, 10);
    m_executorInterface->onLogoutRequest(user, accountId);
}

void protocol_t::parseCharacterCreateRequest(dataServerUser_t *user,
        const eMUCore::packet_t &packet) const {
    unsigned int connectionStamp = packet.read<unsigned int>(4);
    std::string accountId = packet.readString(8, 10);
    std::string name = packet.readString(18, 10);
    eMUShared::characterRace_e::type_t race = static_cast<eMUShared::characterRace_e::type_t>(packet.read<unsigned char>(28));

    m_executorInterface->onCharacterCreateRequest(user,
            connectionStamp,
            accountId,
            name,
            race);
}

void protocol_t::sendCharacterCreateAnswer(dataServerUser_t *user,
        unsigned int connectionStamp,
        const std::string &name,
        eMUShared::characterRace_e::type_t race,
        unsigned char result) const {
    eMUCore::packet_t packet;
    packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
    packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_create);
    packet.insert<unsigned int>(4, connectionStamp);
    packet.insertString(8, name, 10);
    packet.insert<unsigned char>(18, race);
    packet.insert<unsigned char>(19, result);

    m_sendCallback(user, packet);
}



void protocol_t::parseCharacterDeleteRequest(dataServerUser_t *user,
        const eMUCore::packet_t &packet) const {
    unsigned int connectionStamp = packet.read<unsigned int>(4);
    std::string accountId = packet.readString(8, 10);
    std::string name = packet.readString(18, 10);
    std::string pin = packet.readString(28, 7);

    m_executorInterface->onCharacterDeleteRequest(user, connectionStamp, accountId, name, pin);
}

void protocol_t::sendCharacterDeleteAnswer(dataServerUser_t *user,
        unsigned int connectionStamp,
        const std::string &name,
        unsigned char result) const {
    eMUCore::packet_t packet;
    packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
    packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_delete);
    packet.insert<unsigned int>(4, connectionStamp);
    packet.insertString(8, name, 10);
    packet.insert<unsigned char>(18, result);

    m_sendCallback(user, packet);
}

void protocol_t::parseCharacterSelectRequest(dataServerUser_t *user,
        const eMUCore::packet_t &packet) const {
    unsigned int connectionStamp = packet.read<unsigned int>(4);
    std::string accountId = packet.readString(8, 10);
    std::string name = packet.readString(18, 10);

    m_executorInterface->onCharacterSelectRequest(user, connectionStamp, accountId, name);
}

void protocol_t::sendCharacterSelectAnswer(dataServerUser_t *user,
        unsigned int connectionStamp,
        const eMUShared::characterAttributes_t &attr) const {
    eMUCore::packet_t packet;
    packet.construct(0xC1, eMUShared::dataServerProtocol_e::_characterManage);
    packet.insert<unsigned char>(3, eMUShared::dataServerProtocol_e::characterManage_e::_select);
    packet.insert<unsigned int>(4, connectionStamp);
    packet.insertString(8, attr.m_name, 10);
    packet.insert<unsigned char>(18, attr.m_race);
    packet.insert<unsigned char>(19, attr.m_position.m_x);
    packet.insert<unsigned char>(20, attr.m_position.m_y);
    packet.insert<unsigned char>(21, attr.m_mapId);
    packet.insert<unsigned char>(22, attr.m_direction);
    packet.insert<unsigned int>(23, attr.m_experience);
    packet.insert<unsigned short>(27, attr.m_levelUpPoints);
    packet.insert<unsigned short>(29, attr.m_level);
    packet.insert<unsigned short>(31, attr.m_strength);
    packet.insert<unsigned short>(33, attr.m_agility);
    packet.insert<unsigned short>(35, attr.m_vitality);
    packet.insert<unsigned short>(37, attr.m_energy);
    packet.insert<unsigned short>(39, attr.m_health);
    packet.insert<unsigned short>(41, attr.m_maxHealth);
    packet.insert<unsigned short>(43, attr.m_mana);
    packet.insert<unsigned short>(45, attr.m_maxMana);
    packet.insert<unsigned short>(47, attr.m_shield);
    packet.insert<unsigned short>(49, attr.m_maxShield);
    packet.insert<unsigned short>(51, attr.m_stamina);
    packet.insert<unsigned short>(53, attr.m_maxStamina);
    packet.insert<unsigned int>(55, attr.m_money);
    packet.insert<unsigned char>(59, attr.m_pkLevel);
    packet.insert<unsigned char>(60, attr.m_controlCode);
    packet.insert<unsigned short>(61, attr.m_addPoints);
    packet.insert<unsigned short>(63, attr.m_maxAddPoints);
    packet.insert<unsigned short>(65, attr.m_command);
    packet.insert<unsigned short>(67, attr.m_minusPoints);
    packet.insert<unsigned short>(69, attr.m_maxMinusPoints);

    m_sendCallback(user, packet);
}

void protocol_t::parseCharacterSaveRequest(dataServerUser_t *user,
        const eMUCore::packet_t &packet) const {
    std::string accountId = packet.readString(4, 10);

    eMUShared::characterAttributes_t attr;
    attr.m_name = packet.readString(14, 10);
    attr.m_race = static_cast<eMUShared::characterRace_e::type_t>(packet.read<unsigned char>(24));
    attr.m_position.set(packet.read<unsigned char>(25), packet.read<unsigned char>(26));
    attr.m_mapId = packet.read<unsigned char>(27);
    attr.m_direction = packet.read<unsigned char>(28);
    attr.m_experience = packet.read<unsigned int>(29);
    attr.m_levelUpPoints = packet.read<unsigned short>(33);
    attr.m_level = packet.read<unsigned short>(35);
    attr.m_strength = packet.read<unsigned short>(37);
    attr.m_agility = packet.read<unsigned short>(39);
    attr.m_vitality = packet.read<unsigned short>(41);
    attr.m_energy = packet.read<unsigned short>(43);
    attr.m_health = packet.read<unsigned short>(45);
    attr.m_maxHealth = packet.read<unsigned short>(47);
    attr.m_mana = packet.read<unsigned short>(49);
    attr.m_maxMana = packet.read<unsigned short>(51);
    attr.m_shield = packet.read<unsigned short>(53);
    attr.m_maxShield = packet.read<unsigned short>(55);
    attr.m_stamina = packet.read<unsigned short>(57);
    attr.m_maxStamina = packet.read<unsigned short>(59);
    attr.m_money = packet.read<unsigned int>(61);
    attr.m_pkLevel = packet.read<unsigned char>(65);
    attr.m_controlCode = packet.read<unsigned char>(66);
    attr.m_addPoints = packet.read<unsigned short>(67);
    attr.m_maxAddPoints = packet.read<unsigned short>(69);
    attr.m_command = packet.read<unsigned short>(71);
    attr.m_minusPoints = packet.read<unsigned short>(73);
    attr.m_maxMinusPoints = packet.read<unsigned short>(75);

    m_executorInterface->onCharacterSaveRequest(user, accountId, attr);
}

void protocol_t::sendQueryExceptionNotice(dataServerUser_t *user,
        unsigned int connectionStamp,
        const std::string &what) const {
    eMUCore::packet_t packet;
    packet.construct(0xC2, eMUShared::dataServerProtocol_e::_queryException);
    packet.insert<unsigned int>(4, connectionStamp);
    packet.insertString(8, what, 512);

    m_sendCallback(user, packet);
}