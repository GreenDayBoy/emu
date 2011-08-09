#ifndef eMU_CONNECTSERVER_SERVERLIST_H
#define eMU_CONNECTSERVER_SERVERLIST_H

#include <map>
#include <string>
#include "..\core\core.h"

class serverList_t {
public:
    struct serverAttributes_t {
        std::string		m_name;
        std::string		m_address;
        unsigned short	m_code;
        unsigned short	m_port;
        size_t			m_load;
        time_t			m_lastActivityTime;
        bool			m_active;

        friend std::ostream& operator<<(std::ostream &out, const serverList_t::serverAttributes_t &attr) {
            out << "code: " << attr.m_code << ", name: " << attr.m_name << ", address: " << attr.m_address << ", port: " << attr.m_port << "";
            return out;
        }
    };

    typedef std::map<int, serverAttributes_t> serverAttributesList_t;

    serverList_t(time_t serverTimeout);

    void startup(const std::string &fileName);
    void update();
    void serverUpdate(int serverCode, size_t load);

    inline const serverAttributesList_t& list() const {
        return m_list;
    }
    inline size_t activeServersCount() const {
        return m_activeServersCount;
    }
    const serverAttributes_t& serverAttributes(unsigned short code) const;

    void logger(eMUCore::logger_t *logger) {
        m_logger = logger;
    }

private:
    serverList_t();
    serverList_t(const serverList_t&);
    serverList_t& operator=(const serverList_t&);

    serverAttributesList_t	m_list;
    eMUCore::logger_t		*m_logger;
    time_t					m_serverTimeout;
    size_t					m_activeServersCount;
};

#endif // eMU_CONNECTSERVER_SERVERLIST_H