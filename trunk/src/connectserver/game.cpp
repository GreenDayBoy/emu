#include "game.h"
#include <boost/bind.hpp>

game_t::game_t(const disconnectCallback_t &disconnectCallback):
    m_logger(NULL),
    m_scheduler(NULL),
    m_disconnectCallback(disconnectCallback),
    m_serverList(5),
    m_protocol(NULL) {}

void game_t::startup() {
    m_serverList.logger(m_logger);
    m_serverList.startup("serverList.xml");

    m_scheduler->insert(eMUCore::schedule_e::_synchronized,
                        boost::bind(&serverList_t::update, &m_serverList),
                        1);
}

void game_t::onServerListRequest(connectServerUser_t *user) {
    if(m_serverList.activeServersCount() > 0) {
        m_protocol->sendServerListAnswer(user, m_serverList.list());
    } else {
        m_logger->in(eMUCore::loggerMessage_e::_warning) << *user << " No active gameservers.";
        m_logger->out();

        m_disconnectCallback(user);
    }
}

void game_t::onServerSelectRequest(connectServerUser_t *user, unsigned short serverCode) {
#ifdef _DEBUG
    m_logger->in(eMUCore::loggerMessage_e::_debug) << *user << " Selected serverCode: " << serverCode << ".";
    m_logger->out();
#endif

    const serverList_t::serverAttributes_t &attr = m_serverList.serverAttributes(serverCode);

    m_logger->in(eMUCore::loggerMessage_e::_info) << *user << " Selected server " << attr << ".";
    m_logger->out();

    m_protocol->sendServerSelectAnswer(user, attr);
}