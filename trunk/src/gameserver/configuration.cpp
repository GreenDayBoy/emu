#include <boost\algorithm\string.hpp>
#include "configuration.h"
#include "..\core\core.h"

void serverConfiguration_t::read(const std::string &configFileName) {
	eMUCore::xmlConfig_t configFile;
	configFile.open(configFileName, "configuration");

	m_serverCode = configFile.readFromNode<unsigned short>("serverCode", static_cast<unsigned short>(-1));

	m_connectServerHost = configFile.readFromNode<std::string>("connectServerHost", "localhost");
	m_connectServerPort = configFile.readFromNode<unsigned short>("connectServerPort", 44405);

	m_masterDataServerHost = configFile.readFromNode<std::string>("masterDataServerHost", "localhost");
	m_masterDataServerPort = configFile.readFromNode<unsigned short>("masterDataServerPort", 55960);

	m_slaveDataServerHost = configFile.readFromNode<std::string>("slaveDataServerHost", "localhost");
	m_slaveDataServerPort = configFile.readFromNode<unsigned short>("slaveDataServerPort", 55962);

	configFile.close();
}

void versionConfiguration_t::read(const std::string &versionFileName) {
	eMUCore::xmlConfig_t versionFile;
	versionFile.open(versionFileName, "version");

	m_version = versionFile.readFromNode<std::string>("clientExeVersion", "1.02.06");
	m_versionProtocol = boost::algorithm::replace_all_copy(m_version, ".", "");
	m_serial = versionFile.readFromNode<std::string>("clientExeSerial", "adkxNILNDL35skKh");

	versionFile.close();
}

void gameConfiguration_t::read(const std::string &gameFileName) {
	eMUCore::xmlConfig_t gameFile;
	gameFile.open(gameFileName, "game");

	m_advancedRaceLevel = gameFile.readFromNode<unsigned short>("advancedRaceLevel", 220);
	m_welcomeNotice = gameFile.readFromNode<std::string>("welcomeNotice", "Welcome!");

	gameFile.close();
}