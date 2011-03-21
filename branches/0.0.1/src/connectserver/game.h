#ifndef eMU_CONNECTSERVER_GAME_H
#define eMU_CONNECTSERVER_GAME_H

#include "..\core\core.h"
#include "protocol.h"
#include "serverlist.h"
#include "user.h"

class game_t: public protocolExecutorInterface_t {
public:
	typedef boost::function1<void, connectServerUser_t&> disconnectCallback_t;

	game_t(eMUCore::logger_t &logger,
			eMUCore::scheduler_t &scheduler,
			protocol_t &protocol,
			const disconnectCallback_t &disconnectCallback);

	void startup();

	inline void serverListUpdate(int serverCode, 
									size_t load) { m_serverList.serverUpdate(serverCode, load); }

	// ----------------------------------------------------
	// Interface for protocolExecutor_t.
	void onServerListRequest(connectServerUser_t &user);

	void onServerSelectRequest(connectServerUser_t &user,
								unsigned short serverCode);
	// ----------------------------------------------------

private:
	game_t();
	game_t(const game_t &game);
	game_t& operator=(const game_t &game);

	disconnectCallback_t		m_disconnectCallback;
	protocol_t					&m_protocol;
	eMUCore::logger_t			&m_logger;
	eMUCore::scheduler_t		&m_scheduler;
	serverList_t				m_serverList;
};

#endif // eMU_CONNECTSERVER_GAME_H