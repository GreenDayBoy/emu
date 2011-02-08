#include "serverlist.h"

serverList_t::serverList_t(eMUCore::logger_t &logger,
							time_t serverTimeout):
  m_logger(logger),
  m_serverTimeout(serverTimeout),
  m_activeServersCount(0) {}

void serverList_t::startup(const std::string &fileName) {
	eMUCore::xmlConfig_t listFile;
	listFile.open(fileName.c_str(), "servers");
	
	while(listFile.nextNode()) {
		serverAttributes_t attr;

		attr.m_code = listFile.readFromProperty<unsigned short>("server", "code", static_cast<unsigned short>(-1));
		attr.m_name = listFile.readFromProperty<std::string>("server", "name", "defaultName");
		attr.m_address = listFile.readFromProperty<std::string>("server", "address", "defaultAddress");
		attr.m_port = listFile.readFromProperty<unsigned short>("server", "port", 0);
		attr.m_active = false;
		attr.m_load = 0;
		attr.m_lastActivityTime = 0;

		#ifdef _DEBUG
		m_logger.in(eMUCore::logger_t::_MESSAGE_DEBUG) << "[serverList_t::startup()] Read " << attr << ".";
		m_logger.out();
		#endif

		m_list[attr.m_code] = attr;
	}
}

void serverList_t::update() {
	for(serverAttributesList_t::iterator i = m_list.begin(); i != m_list.end(); ++i) {
		time_t tickCount = GetTickCount();

		serverAttributes_t &attr = i->second;

		if(tickCount - attr.m_lastActivityTime > m_serverTimeout * 1000) {
			if(attr.m_active) {
				m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << attr << " Server is timed out.";
				m_logger.out();

				if(m_activeServersCount > 0) {
					--m_activeServersCount;
				}
			}

			attr.m_active = false;
		} else {
			if(!attr.m_active) {
				m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << attr << " Server is alive.";
				m_logger.out();
				++m_activeServersCount;
			}

			attr.m_active = true;
		}
	}
}

void serverList_t::serverUpdate(int code, size_t load) {
	serverAttributesList_t::iterator iter = m_list.find(code);

	if(iter != m_list.end()) {
		iter->second.m_load = load;
		iter->second.m_lastActivityTime = GetTickCount();
	}
}

const serverList_t::serverAttributes_t& serverList_t::getServerAttributes(unsigned short code) const {
	serverAttributesList_t::const_iterator iter = m_list.find(code);

	if(iter != m_list.end()) {
		return iter->second;
	} else {
		eMUCore::exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << "[serverList_t::getServerAttributes()] No server [" << code << "] found on servers list.";
		throw e;
	}
}