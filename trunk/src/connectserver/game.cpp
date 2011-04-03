#include "game.h"
#include <boost/bind.hpp>

game_t::game_t(eMUCore::logger_t &logger,
				eMUCore::scheduler_t &scheduler,
				protocol_t &protocol,
				const disconnectCallback_t &disconnectCallback):
  m_logger(logger),
  m_scheduler(scheduler),
  m_disconnectCallback(disconnectCallback),
  m_serverList(m_logger, 5),
  m_protocol(protocol) {}

void game_t::startup() {
	m_serverList.startup("serverList.xml");

	m_scheduler.insert(eMUCore::schedule_e::_synchronized,
						boost::bind(&serverList_t::update, &m_serverList),
						1);
}

void game_t::onServerListRequest(connectServerUser_t &user) {
	if(m_serverList.getActiveServersCount() > 0) {
		m_protocol.sendServerListAnswer(user, m_serverList.getList());
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_warning) << user << " No active gameservers.";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onServerSelectRequest(connectServerUser_t &user, unsigned short serverCode) {
	#ifdef _DEBUG
	m_logger.in(eMUCore::loggerMessage_e::_debug) << user << " Selected serverCode [" << serverCode << "].";
	m_logger.out();
	#endif

	const serverList_t::serverAttributes_t &attr = m_serverList.getServerAttributes(serverCode);

	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Selected server " << attr << ".";
	m_logger.out();

	m_protocol.sendServerSelectAnswer(user, attr);
}