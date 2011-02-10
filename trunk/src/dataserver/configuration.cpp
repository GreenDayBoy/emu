#include <algorithm>
#include "configuration.h"
#include "..\core\core.h"

void configuration_t::read(const std::string &configFileName) {
	eMUCore::xmlConfig_t configFile;
	configFile.open(configFileName, "configuration");

	m_dbHostname = configFile.readFromNode<std::string>("dbHostname", "localhost");
	m_dbPort = configFile.readFromNode<unsigned short>("dbPort", 3306);
	m_dbName = configFile.readFromNode<std::string>("dbName", "muonline");
	m_dbUserName = configFile.readFromNode<std::string>("dbUserName", "root");
	m_dbPassword = configFile.readFromNode<std::string>("dbPassword", "");

	configFile.close();
}

void allowedHostList_t::read(const std::string &hostsFileName) {
	eMUCore::xmlConfig_t hostsFile;
	hostsFile.open(hostsFileName, "hosts");

	while(hostsFile.nextNode()) {
		std::string hostname = hostsFile.readFromProperty<std::string>("host", "address", "127.0.0.1");
		m_list.push_back(hostname);
	}

	hostsFile.close();
}

bool allowedHostList_t::isHostAllowed(const std::string &ipAddress) const {
	for(std::vector<std::string>::const_iterator i = m_list.begin(); i != m_list.end(); ++i) {
		if(eMUCore::isIpAddress(*i)) {
			if(*i == ipAddress) {
				return true;
			}
		} else {
			if(eMUCore::convertToIpAddress(*i) == ipAddress) {
				return true;
			}
		}
	}

	return false;
}