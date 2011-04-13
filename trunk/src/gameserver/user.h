#ifndef eMU_GAMESERVER_USER_H
#define eMU_GAMESERVER_USER_H

#include <vector>
#include "..\core\core.h"
#include "..\shared\shared.h"
#include "character.h"

#pragma warning(disable: 4355)

class gameServerUser_t: public eMUCore::socketContext_t {
public:
	gameServerUser_t(int index);

	friend std::ostream& operator<<(std::ostream &out, const gameServerUser_t &user) {
		out << reinterpret_cast<const eMUCore::socketContext_t&>(user);

		if(user.m_accountId != "") {
			out	<< "[" << user.m_accountId << "]";
		}

		return out;
	}

	void reset();

	inline unsigned char generateCryptSerial() { return m_cryptSerial++; }

	inline void incerementLoginAttempts() { ++m_loginAttempts; }
	inline int loginAttempts() const { return m_loginAttempts; }

	inline void accountId(const std::string &accountId) { m_accountId = accountId; }
	inline const std::string& accountId() const { return m_accountId; }

	inline void logIn() { m_loggedIn = true; }
	inline bool loggedIn() const { return m_loggedIn; }

	void availableRaces(const eMUShared::characterList_t &characterList,
							unsigned short advancedRaceLevel);
	inline availableRace_e::type_t availableRaces() const { return m_availableRaces; }

	inline void closeReason(clientCloseReason_e::type_t closeReason) { m_closeReason = closeReason; }
	inline void resetCloseReason() { m_closeReason = clientCloseReason_e::_none; }
	inline unsigned char closeReason() const { return m_closeReason; }

	inline void timeToClose(int timeToClose) { m_timeToClose = timeToClose; }
	inline void decrecemntTimeToClose() { --m_timeToClose; }
	inline int timeToClose() const { return m_timeToClose; }

	inline void connectionStamp(unsigned int connectionStamp) { m_connectionStamp = connectionStamp; }
	inline unsigned int connectionStamp() const { return m_connectionStamp; }

	inline character_t& character() { return m_character; }

	void initializeCharacterListMap();
	void mapCharacterList(const eMUShared::characterList_t &characterList);
	int insertToCharacterList(const std::string &name);
	void deleteFromCharacterList(const std::string &name);

	bool operator==(unsigned int connectionStamp) { return m_connectionStamp == connectionStamp; }
	bool operator==(const std::string &characterName);

private:
	std::string							m_accountId;
	std::map<unsigned int, std::string>	m_characterListMap;
	unsigned char						m_cryptSerial;
	int									m_loginAttempts;
	bool								m_loggedIn;
	availableRace_e::type_t				m_availableRaces;
	clientCloseReason_e::type_t			m_closeReason;
	int									m_timeToClose;
	unsigned int						m_connectionStamp;
	character_t							m_character;

	gameServerUser_t();
	gameServerUser_t(const gameServerUser_t&);
	gameServerUser_t& operator=(const gameServerUser_t&);
};

#pragma warning(default: 4355)

#endif // eMU_GAMESERVER_USER_H