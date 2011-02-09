#ifndef eMU_GAMESERVER_GAMESERVER_H
#define eMU_GAMESERVER_GAMESERVER_H

#include <boost/bind.hpp>
#include "..\core\core.h"
#include "user.h"
#include "game.h"
#include "configuration.h"
#include "crypt.h"

typedef std::vector<eMUCore::packet_t> dataServerPacketQueue_t;

class gameServer_t {
public:
	gameServer_t(size_t maxStoredLogsCount,
					size_t usersMax,
					unsigned short port);
	~gameServer_t();

	void startup();
	void worker();
	void updateWindowTitle() const;

	gameServerUser_t* onContextAllocate();

	void onContextAttach(eMUCore::socketContext_t &context);
	void onContextReceive(eMUCore::socketContext_t &context);
	void onContextClose(eMUCore::socketContext_t &context);

	void onDatagramReceive(sockaddr_in& /*inetAddr*/, unsigned char * /*data*/, size_t /*dataSize*/) {}

	void onTcpClientConnect();
	void onTcpClientReceive();
	void onTcpClientClose();

	void send(gameServerUser_t &user, eMUCore::packet_t &packet);
	void sendDataServer(const eMUCore::packet_t &packet);
	void disconnect(gameServerUser_t &user) { m_iocpEngine.detach(user); }

	// metody sa statyczne z powodu console ctrl handlera...
	static void fatalHandler(int signalId);
	static DWORD WINAPI consoleEventHandler(unsigned int eventId);
	inline static void stop() { m_interrupt = true; }

private:
	gameServer_t();
	gameServer_t(const gameServer_t&);
	gameServer_t& operator=(const gameServer_t&);

	void dataServerConnect();
	void disconnectAll();
	void sendServerInfo() const;

	eMUCore::socketContextManager_t<gameServerUser_t>	m_userManager;
	eMUCore::logger_t									m_logger;
	eMUCore::synchronizer_t								m_synchronizer;
	eMUCore::scheduler_t								m_scheduler;
	eMUCore::tcpClient_t								m_tcpClient;
	eMUCore::iocpEngine_t								m_iocpEngine;
	eMUCore::tcpServer_t								m_tcpServer;
	eMUCore::udpSocket_t								m_udpSocket;
	dataServerPacketQueue_t								m_packetQueue;
	serverConfiguration_t								m_serverConfiguration;
	versionConfiguration_t								m_versionConfiguration;
	protocol_t											m_protocol;
	dataServerProtocol_t								m_dataServerProtocol;
	crypt_t												m_crypt;
	game_t												m_game;
	size_t												m_userCount;
	static bool											m_interrupt; // zmienna jest statyczna z powodu console ctrl handlera...
};

void main(int argsCount, char *argsVect[]);

#endif // eMU_GAMESERVER_GAMESERVER_H