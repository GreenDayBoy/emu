#ifndef eMU_GAMESERVER_CONFIGURATION_H
#define eMU_GAMESERVER_CONFIGURATION_H

#include <string>

struct serverConfiguration_t {
public:
	serverConfiguration_t() {}

	void read(const std::string &configFileName);

	unsigned short	m_serverCode;
	std::string		m_connectServerHost;
	unsigned short	m_connectServerPort;

	std::string		m_masterDataServerHost;
	unsigned short	m_masterDataServerPort;

	std::string		m_slaveDataServerHost;
	unsigned short	m_slaveDataServerPort;

private:
	serverConfiguration_t(const serverConfiguration_t&);
	serverConfiguration_t& operator=(const serverConfiguration_t&);
};

struct versionConfiguration_t {
public:
	versionConfiguration_t() {}

	void read(const std::string &versionFileName);

	std::string m_version;
	std::string	m_versionProtocol;
	std::string m_serial;

private:
	versionConfiguration_t(const versionConfiguration_t&);
	versionConfiguration_t& operator=(const versionConfiguration_t&);
};

struct gameConfiguration_t {
public:
	gameConfiguration_t() {}

	void read(const std::string &gameFileName);

	unsigned short m_advancedRaceLevel;
	std::string m_welcomeNotice;

private:
	gameConfiguration_t(const gameConfiguration_t&);
	gameConfiguration_t& operator=(const gameConfiguration_t&);
};

#endif // eMU_GAMESERVER_CONFIGURATION_H