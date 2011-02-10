#ifndef eMU_DATASERVER_DATASERVER_H
#define eMU_DATASERVER_DATASERVER_H

#include <boost/bind.hpp>
#include "..\core\core.h"
#include "user.h"
#include "game.h"
#include "configuration.h"

class dataServer_t {
public:
	dataServer_t(size_t maxStoredLogsCount,
					size_t userMax,
					unsigned short port);
	void startup();
	void cleanup();
	void worker();
	void updateWindowTitle() const;
	dataServerUser_t* onContextAllocate();
	void onContextAttach(eMUCore::socketContext_t &context);
	void onContextReceive(eMUCore::socketContext_t &context);
	void onContextClose(eMUCore::socketContext_t &context);
	void send(dataServerUser_t &user, const eMUCore::packet_t &packet);
	void disconnect(dataServerUser_t &user) { m_iocpEngine.detach(user); }

	// metody sa statyczne z powodu console ctrl handlera...
	static void fatalHandler(int signalId);
	static DWORD WINAPI consoleEventHandler(unsigned int eventId);
	inline static void stop() { m_interrupt = true; }

private:
	dataServer_t();
	dataServer_t(const dataServer_t&);
	dataServer_t& operator=(const dataServer_t&);

	void disconnectAll();

	eMUCore::socketContextManager_t<dataServerUser_t>	m_userManager;
	eMUCore::logger_t									m_logger;
	eMUCore::synchronizer_t								m_synchronizer;
	eMUCore::scheduler_t								m_scheduler;
	eMUCore::iocpEngine_t								m_iocpEngine;
	eMUCore::tcpServer_t								m_tcpServer;
	allowedHostList_t									m_allowedHostList;
	protocol_t											m_protocol;
	game_t												m_game;
	size_t												m_userCount;
	static bool											m_interrupt; // zmienna jest statyczna z powodu console ctrl handlera...
};

void main(int argsCount, char *argsVect[]);

#endif // eMU_DATASERVER_DATASERVER_H