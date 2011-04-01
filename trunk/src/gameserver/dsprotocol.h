#ifndef eMU_GAMESERVER_DSPROTOCOL_H
#define eMU_GAMESERVER_DSPROTOCOL_H

#include "..\core\core.h"
#include "..\shared\shared.h"
#include "enum.h"

// what a big name xD.
class dataServerProtocolExecutorInterface_t {
public:
	virtual void onAccountCheckAnswer(unsigned int connectionStamp,
										const std::string &accountId,
										accountCheckResult_e::type_t result) = 0;

	virtual void onCharacterListAnswer(unsigned int connectionStamp,
										const eMUShared::characterList_t &characterList) = 0;

	virtual void onCharacterCreateAnswer(unsigned int connectionStamp,
											const std::string &name,
											eMUShared::characterRace_e::type_t race,
											characterCreateResult_e::type_t result) = 0;

	virtual void onCharacterDeleteAnswer(unsigned int connectionStamp,
											const std::string &name,
											characterDeleteResult_e::type_t result) = 0;

	virtual void onCharacterSelectAnswer(unsigned int connectionStamp,
											const eMUShared::characterAttributes_t &attr) = 0;

	virtual void onQueryExceptionNotice(unsigned int connectionStamp,
									const std::string &what) = 0;
};

class dataServerProtocol_t {
public:
	typedef boost::function1<void, const eMUCore::packet_t&> sendCallback_t;

	dataServerProtocol_t(dataServerProtocolExecutorInterface_t &iface,
							const sendCallback_t &sendCallback);

	void core(const eMUCore::packet_t &packet) const;

	void sendAccountCheckRequest(unsigned int connectionStamp,
									const std::string &accountId,
									const std::string &password,
									const std::string &ipAddress) const;

	void parseAccountCheckAnswer(const eMUCore::packet_t &packet) const;

	void sendCharacterListRequest(unsigned int connectionStamp,
									const std::string &accountId) const;

	void parseCharacterListAnswer(const eMUCore::packet_t &packet) const;

	void sendLogoutRequest(const std::string &accountId) const;

	void sendCharacterCreateRequest(unsigned int connectionStamp,
									const std::string &accountId,
									const std::string &name,
									eMUShared::characterRace_e::type_t race) const;

	void parseCharacterCreateAnswer(const eMUCore::packet_t &packet) const;

	void sendCharacterDeleteRequest(unsigned int connectionStamp,
									const std::string &accountId,
									const std::string &name,
									const std::string &pin) const;

	void parseCharacterDeleteAnswer(const eMUCore::packet_t &packet) const;

	void sendCharacterSelectRequest(unsigned int connectionStamp,
									const std::string &accountId,
									const std::string &name) const;

	void parseCharacterSelectAnswer(const eMUCore::packet_t &packet) const;

	void sendCharacterSaveRequest(const std::string &accountId,
									const eMUShared::characterAttributes_t &attr) const;

	void parseQueryExceptionNotice(const eMUCore::packet_t &packet) const;

private:
	dataServerProtocol_t();
	dataServerProtocol_t(const dataServerProtocol_t&);
	dataServerProtocol_t& operator=(const dataServerProtocol_t&);

	dataServerProtocolExecutorInterface_t	&m_executorInterface;
	sendCallback_t							m_sendCallback;
};

#endif // eMU_GAMESERVER_DSPROTOCOL_H