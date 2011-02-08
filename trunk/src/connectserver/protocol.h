#ifndef eMU_CONNECTSERVER_PROTOCOL_H
#define eMU_CONNECTSERVER_PROTOCOL_H

#include "..\core\core.h"
#include "user.h"
#include "serverlist.h"

class protocolExecutorInterface_t {
public:
	virtual ~protocolExecutorInterface_t() {}
	virtual void onServerListRequest(connectServerUser_t &user) = 0;
	virtual void onServerSelectRequest(connectServerUser_t &user, 
										unsigned short serverCode) = 0;
};

class protocol_t {
public:
	typedef boost::function2<void, connectServerUser_t&, const eMUCore::packet_t&> sendCallback_t;

	protocol_t(protocolExecutorInterface_t &iface,
				const sendCallback_t &sendCallback);

	void core(connectServerUser_t &user, 
				const eMUCore::packet_t &packet) const;

	void sendHandshake(connectServerUser_t &user) const;

	void parseServerListRequest(connectServerUser_t &user, 
								const eMUCore::packet_t &packet) const;

	void sendServerListAnswer(connectServerUser_t &user, 
								const serverList_t::serverAttributesList_t &list) const;

	void parseServerSelectRequest(connectServerUser_t &user, 
									const eMUCore::packet_t &packet) const;

	void sendServerSelectAnswer(connectServerUser_t &user, 
								const serverList_t::serverAttributes_t &attr) const;

private:
	protocol_t();
	protocol_t(const protocol_t&);
	protocol_t& operator=(const protocol_t&);

	protocolExecutorInterface_t &m_executorInterface;
	sendCallback_t				m_sendCallback;
};

#endif // eMU_CONNECTSERVER_PROTOCOL_H