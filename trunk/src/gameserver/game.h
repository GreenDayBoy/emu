#ifndef eMU_GAMESERVER_GAME_H
#define eMU_GAMESERVER_GAME_H

#include "protocol.h"
#include "dsprotocol.h"
#include "user.h"
#include "configuration.h"
#include "map.h"
#include "gate.h"
#include "monster.h"
#include "viewport.h"
#include "item.h"

#pragma warning(disable: 4355)

class game_t: public protocolExecutorInterface_t,
				public dataServerProtocolExecutorInterface_t {
public:
	typedef boost::function1<void, gameServerUser_t&> disconnectCallback_t;

	game_t(eMUCore::logger_t &logger,
			eMUCore::socketContextManager_t<gameServerUser_t> &userManager,
			eMUCore::scheduler_t &scheduler,
			protocol_t &protocol,
			dataServerProtocol_t &dataServerProtocol,
			versionConfiguration_t &versionConfiguration,
			const disconnectCallback_t &disconnectCallback);

	void startup();
	void cleanup();

	void onClientClose(gameServerUser_t &user);
	void onCharacterLeave(gameServerUser_t &user);

	void checkSelfClose();
	void saveCharacter(gameServerUser_t &user) const;
	void teleportCharacter(gameServerUser_t &user,
							unsigned char mapId,
							unsigned char x,
							unsigned char y,
							unsigned char direction,
							unsigned char gateId = 1);

	// --------------------------------------------------------
	// Interface for protocolExecutor_t.
	void onLoginRequest(gameServerUser_t &user,
							const std::string &accountId,
							const std::string &password,
							const std::string &clientExeVersion,
							const std::string &clientExeSerial);
	void onCharacterListRequest(gameServerUser_t &user);
	void onLogoutRequest(gameServerUser_t &user, clientCloseReason_e::type_t closeReason);
	void onCharacterCreateRequest(gameServerUser_t &user,
									const std::string &name,
									eMUShared::characterRace_e::type_t race);
	void onCharacterDeleteRequest(gameServerUser_t &user,
									const std::string &name,
									const std::string &pin);
	void onCharacterSelectRequest(gameServerUser_t &user,
									const std::string &name);
	void onCharacterMoveRequest(gameServerUser_t &user,
									unsigned char x,
									unsigned char y,
									unsigned char direction,
									const map_t::path_t &path);
	void onCharacterTeleportRequest(gameServerUser_t &user,
									unsigned short gateId);
	void onPublicChat(gameServerUser_t& user,
						const std::string &message);
	void onCharacterAction(gameServerUser_t &user,
							unsigned char direction,
							characterAction_e::type_t actionId);
	// --------------------------------------------------------

	// --------------------------------------------------------
	// Interface for dataServerProtocolExecutor_t.
	void onAccountCheckAnswer(unsigned int connectionStamp,
								const std::string &accountId,
								accountCheckResult_e::type_t result);

	void onCharacterListAnswer(unsigned int connectionStamp,
								const eMUShared::characterList_t &characterList);

	void onCharacterCreateAnswer(unsigned int connectionStamp,
									const std::string &name,
									eMUShared::characterRace_e::type_t race,
									characterCreateResult_e::type_t result);

	void onCharacterDeleteAnswer(unsigned int connectionStamp,
									const std::string &name,
									characterDeleteResult_e::type_t result);

	void onCharacterSelectAnswer(unsigned int connectionStamp,
									const eMUShared::characterAttributes_t &attr);

	void onQueryExceptionNotice(unsigned int connectionStamp,
							const std::string &what);
	// --------------------------------------------------------

private:
	game_t();
	game_t(const game_t&);
	game_t& operator=(const game_t&);

	eMUCore::logger_t									&m_logger;
	eMUCore::scheduler_t								&m_scheduler;
	eMUCore::socketContextManager_t<gameServerUser_t>	&m_userManager;
	protocol_t											&m_protocol;
	dataServerProtocol_t								&m_dataServerProtocol;
	versionConfiguration_t								&m_versionConfiguration;
	gameConfiguration_t									m_gameConfiguration;
	mapManager_t										m_mapManager;
	gateManager_t										m_gateManager;
	monsterAttributesManager_t							m_monsterAttributesManager;
	monsterManager_t									m_monsterManager;
	viewportManager_t									m_viewportManager;
	itemAttributesManager_t								m_itemAttributesManager;
	disconnectCallback_t								m_disconnectCallback;
};

#pragma warning(default: 4355)

#endif // eMU_GAMESERVER_GAME_H