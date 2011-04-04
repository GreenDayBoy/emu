#ifndef eMU_GAMESERVER_PROTOCOL_H
#define eMU_GAMESERVER_PROTOCOL_H

#include "..\core\core.h"
#include "..\shared\shared.h"
#include "user.h"
#include "character.h"
#include "monster.h"
#include "map.h"
#include "viewport.h"

class protocolExecutorInterface_t {
public:
	virtual void onLoginRequest(gameServerUser_t &user,
								const std::string &accountId,
								const std::string &password,
								const std::string &clientExeVersion,
								const std::string &clientExeSerial) = 0;

	virtual void onCharacterListRequest(gameServerUser_t &user) = 0;
	virtual void onLogoutRequest(gameServerUser_t &user, clientCloseReason_e::type_t closeReason) = 0;
	virtual void onCharacterCreateRequest(gameServerUser_t &user,
											const std::string &name,
											eMUShared::characterRace_e::type_t race) = 0;
	virtual void onCharacterDeleteRequest(gameServerUser_t &user,
											const std::string &name,
											const std::string &pin) = 0;
	virtual void onCharacterSelectRequest(gameServerUser_t &user,
											const std::string &name) = 0;
	virtual void onCharacterMoveRequest(gameServerUser_t &user,
										const eMUShared::position_t &pos,
										unsigned char direction,
										const map_t::path_t &path) = 0;
	virtual void onCharacterTeleportRequest(gameServerUser_t &user,
											unsigned short gateId) = 0;
	virtual void onPublicChat(gameServerUser_t& user,
								const std::string &message) = 0;

	virtual void onCharacterAction(gameServerUser_t &user,
									unsigned char direction,
									characterAction_e::type_t actionId) = 0;

};

class protocol_t {
public:
	typedef boost::function2<void, gameServerUser_t&, eMUCore::packet_t&> sendCallback_t;

	protocol_t(protocolExecutorInterface_t &iface,
				const sendCallback_t &sendCallback);

	void core(gameServerUser_t &user, 
				const eMUCore::packet_t &packet) const;

	void sendHandshake(gameServerUser_t &user, unsigned short userIndex, std::string version) const;

	void parseLoginRequest(gameServerUser_t &user,
							const eMUCore::packet_t &packet) const;

	void sendLoginAnswer(gameServerUser_t &user,
							accountCheckResult_e::type_t result) const;

	void parseCharacterListRequest(gameServerUser_t &user,
									const eMUCore::packet_t &packet) const;

	void sendCharacterListAnswer(gameServerUser_t &user,
									availableRace_e::type_t availableRace,
									const eMUShared::characterList_t &characterList) const;

	void parseLogoutRequest(gameServerUser_t &user,
							const eMUCore::packet_t &packet) const;

	void sendLogoutAnswer(gameServerUser_t &user,
							unsigned char closeReason) const;

	void parseCharacterCreateRequest(gameServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendCharacterCreteAnswer(gameServerUser_t &user,
									unsigned char result,
									const std::string &name,
									int slot,
									unsigned short level,
									eMUShared::characterRace_e::type_t race) const;

	void parseCharacterDeleteRequest(gameServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendCharacterDeleteAnswer(gameServerUser_t &user,
									unsigned char result) const;

	void parseCharacterSelectRequest(gameServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendCharacterSelectAnswer(gameServerUser_t &user,
									character_t &character) const;

	void sendTextNotice(gameServerUser_t &user,
						const std::string &notice,
						gameNotice_e::type_t type = gameNotice_e::_gold,
						unsigned char loopCount = 0,
						unsigned short loopDelay = 0,
						unsigned int color = 0,
						unsigned char speed = 0) const;

	void parseCharacterMoveRequest(gameServerUser_t &user,
									const eMUCore::packet_t &packet) const;

	void parseCharacterTeleportRequest(gameServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendCharacterTeleportAnswer(gameServerUser_t &user,
										unsigned char mapId,
										const eMUShared::position_t &pos,
										unsigned char direction,
										unsigned char gateId = 1) const;

	void sendViewportCharacterCreateRequest(gameServerUser_t &user,
												viewportManager_t::viewport_t &viewport) const;

	void sendViewportMonsterCreateRequest(gameServerUser_t &user,
											viewportManager_t::viewport_t &viewport) const;

	void sendViewportDestroyRequest(gameServerUser_t &user,
									viewportManager_t::viewport_t &viewport) const;

	void sendViewportObjectMoveRequest(gameObject_t &object) const;

	void parsePublicChatRequest(gameServerUser_t &user,
								const eMUCore::packet_t &packet) const;

	void sendPublicChatAnswer(gameServerUser_t &user, const std::string &message) const;

	void parseCharacterActionRequest(gameServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendViewportCharacterActionRequest(character_t &character,
											unsigned char actionId,
											unsigned short targetId) const;
private:
	std::string xorString(const std::string &buff) const;

	protocol_t();
	protocol_t(const protocol_t&);
	protocol_t& operator=(const protocol_t&);

	protocolExecutorInterface_t &m_executorInterface;
	sendCallback_t				m_sendCallback;
};

#endif // eMU_GAMESERVER_PROTOCOL_H