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
			out << "[" << attr.m_code << "][" << attr.m_name << "][" << attr.m_address << "][" << attr.m_port << "]";
			return out;
		}
	};

	typedef std::map<int, serverAttributes_t> serverAttributesList_t;

	serverList_t(eMUCore::logger_t &logger,
					time_t serverTimeout);

	void startup(const std::string &fileName);
	void update();
	void serverUpdate(int serverCode, size_t load);

	inline const serverAttributesList_t& getList() const { return m_list; }
	inline size_t getActiveServersCount() const { return m_activeServersCount; }
	const serverAttributes_t& getServerAttributes(unsigned short code) const;

private:
	serverList_t();
	serverList_t(const serverList_t&);
	serverList_t& operator=(const serverList_t&);

	serverAttributesList_t	m_list;
	eMUCore::logger_t		&m_logger;
	time_t					m_serverTimeout;
	size_t					m_activeServersCount;
};

#endif // eMU_CONNECTSERVER_SERVERLIST_H