#ifndef eMU_DATASERVER_PROTOCOL_H
#define eMU_DATASERVER_PROTOCOL_H

#include "..\core\core.h"
#include "..\shared\shared.h"
#include "user.h"

class protocolExecutorInterface_t {
public:
	virtual void onAccountCheckRequest(dataServerUser_t &user,
										unsigned int connectionStamp,
										const std::string &accountId,
										const std::string &password,
										const std::string &ipAddress) = 0;

	virtual void onCharacterListRequest(dataServerUser_t &user,
											unsigned int connectionStamp,
											const std::string &accountId) = 0;

	virtual void onLogoutRequest(dataServerUser_t &user,
									const std::string &accountId) = 0;

	virtual void onCharacterCreateRequest(dataServerUser_t &user,
											unsigned int connectionStamp,
											const std::string &accountId,
											const std::string &name,
											unsigned char race) = 0;

	virtual void onCharacterDeleteRequest(dataServerUser_t &user,
											unsigned int connectionStamp,
											const std::string &accountId,
											const std::string &name,
											const std::string &pin) = 0;

	virtual void onCharacterSelectRequest(dataServerUser_t &user,
											unsigned int connectionStamp,
											const std::string &accountId,
											const std::string &name) = 0;

	virtual void onCharacterSaveRequest(dataServerUser_t &user,
										const std::string &accountId,
										const eMUShared::characterAttributes_t &attr) = 0;
};

class protocol_t {
public:
	typedef boost::function2<void, dataServerUser_t&, const eMUCore::packet_t&> sendCallback_t;

	protocol_t(protocolExecutorInterface_t &iface,
				const sendCallback_t &sendCallback);

	void core(dataServerUser_t &user, 
				const eMUCore::packet_t &packet) const;

	void parseAccountCheckRequest(dataServerUser_t &user,
								const eMUCore::packet_t &packet) const;

	void sendAccountCheckAnswer(dataServerUser_t &user,
								unsigned int connectionStamp,
								const std::string &accountId,
								unsigned char result) const;

	void parseCharacterListRequest(dataServerUser_t &user,
									const eMUCore::packet_t &packet) const;

	void sendCharacterListAnswer(dataServerUser_t &user,
									unsigned int connectionStamp,
									const eMUShared::characterList_t &characterList) const;

	void parseLogoutRequest(dataServerUser_t &user,
							const eMUCore::packet_t &packet) const;

	void parseCharacterCreateRequest(dataServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendCharacterCreateAnswer(dataServerUser_t &user,
									unsigned int connectionStamp,
									const std::string &name,
									unsigned char race,
									unsigned char result) const;

	void parseCharacterDeleteRequest(dataServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendCharacterDeleteAnswer(dataServerUser_t &user,
									unsigned int connectionStamp,
									const std::string &name,
									unsigned char result) const;

	void parseCharacterSelectRequest(dataServerUser_t &user,
										const eMUCore::packet_t &packet) const;

	void sendCharacterSelectAnswer(dataServerUser_t &user,
									unsigned int connectionStamp,
									const eMUShared::characterAttributes_t &attr) const;

	void parseCharacterSaveRequest(dataServerUser_t &user,
									const eMUCore::packet_t &packet) const;

	void sendQueryExceptionNotice(dataServerUser_t &user,
									unsigned int connectionStamp,
									const std::string &what) const;

private:
	protocol_t();
	protocol_t(const protocol_t&);
	protocol_t& operator=(const protocol_t&);

	protocolExecutorInterface_t &m_executorInterface;
	sendCallback_t				m_sendCallback;
};

#endif // eMU_DATASERVER_PROTOCOL_H