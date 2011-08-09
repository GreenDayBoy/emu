#ifndef eMU_DATASERVER_CONFIGURATION_H
#define eMU_DATASERVER_CONFIGURATION_H

#include <string>
#include <vector>

struct configuration_t {
public:
    configuration_t() {}

    void read(const std::string &configFileName);

    std::string		m_dbHostname;
    std::string		m_dbName;
    std::string		m_dbUserName;
    std::string		m_dbPassword;
    unsigned short	m_dbPort;

private:
    configuration_t(const configuration_t&);
    configuration_t& operator=(const configuration_t&);
};

class allowedHostList_t {
public:
    allowedHostList_t() {}

    void read(const std::string &hostsFileName);
    bool isHostAllowed(const std::string &ipAddress) const;

private:
    allowedHostList_t(const allowedHostList_t&);
    allowedHostList_t& operator=(const allowedHostList_t&);

    std::vector<std::string> m_list;
};

#endif // eMU_DATASERVER_CONFIGURATION_H