#include <boost\algorithm\string.hpp>
#include "database.h"

database_t::database_t(eMUCore::logger_t &logger):
  m_logger(logger) {}

void database_t::startup(const std::string &hostname,
							unsigned short port,
							const std::string &dbName,
							const std::string &userName,
							const std::string &password) {
	if(mysql_init(&m_connectionHandle) != NULL) {
		if(mysql_real_connect(&m_connectionHandle,
								hostname.c_str(),
								userName.c_str(),
								password.c_str(),
								dbName.c_str(),
								port,
								0,
								CLIENT_MULTI_RESULTS | CLIENT_MULTI_STATEMENTS) != NULL) {
			bool arg = true;
			mysql_options(&m_connectionHandle, MYSQL_OPT_RECONNECT, &arg);
		} else {
			eMUCore::exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << "[database_t::database_t()] MySQL connect error: " 
					<< mysql_error(&m_connectionHandle);
			throw e;
		}
	} else {
		eMUCore::exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << "[database_t::database_t()] MySQL init error.";
		throw e;
	}
}

void database_t::cleanup() {
	mysql_close(&m_connectionHandle);
}

void database_t::execute() {
	#ifdef _DEBUG
	m_logger.in(eMUCore::loggerMessage_e::_debug) << "Executing query: " << m_queryBuffer.str() << ".";
	m_logger.out();
	#endif

	// SQL Injection protect.
	boost::algorithm::replace_all(m_queryBuffer.str(), "'", "\\'");
	boost::algorithm::replace_all(m_queryBuffer.str(), "\\", "\\\\");

    if(mysql_real_query(&m_connectionHandle,
						m_queryBuffer.str().c_str(),
						m_queryBuffer.str().size()) != 0) {
		//m_logger.in(eMUCore::loggerMessage_e::_error) << "Could not execute query: " << m_queryBuffer.str()
		//												<< ". Reason: " << mysql_error(&m_connectionHandle);
		//m_logger.out();

		eMUCore::exception_t e;
		e.in() << "Could not execute query: " << m_queryBuffer.str() << ". Reason: " << mysql_error(&m_connectionHandle);
		throw e;
	}
}

void database_t::ping() {
	if(mysql_ping(&m_connectionHandle) != 0) {
		m_logger.in(eMUCore::loggerMessage_e::_error) << "Could not ping mysql server. Reason: " << mysql_error(&m_connectionHandle) << ".";
		m_logger.out();
	}
}

MYSQL_RES* database_t::queryResult() {
	MYSQL_RES *queryResult = mysql_store_result(&m_connectionHandle);

	if(queryResult == NULL) {
		//m_logger.in(eMUCore::loggerMessage_e::_warning) << "No result for query: " << m_queryBuffer.str();
		//m_logger.out();

		eMUCore::exception_t e;
		e.in() << "Empty result for query: " << m_queryBuffer.str();
		throw e;
	} else {
		return queryResult;
	}
}

database_t::iterator_t::iterator_t(MYSQL_RES *queryResult):
  m_queryResult(queryResult),
  m_resultRow(NULL),
  m_firstIteration(true) {
	this->mapFields();
	m_resultRow = mysql_fetch_row(m_queryResult);
}

database_t::iterator_t::~iterator_t() {
	mysql_free_result(m_queryResult);
	m_queryResult = NULL;
	m_resultRow = NULL;
	m_fieldMap.clear();
}

bool database_t::iterator_t::nextRow() {
	if(!m_firstIteration) {
		m_resultRow = mysql_fetch_row(m_queryResult);

		if(m_resultRow != NULL) {
			return true;
		} else {
			return false;
		}
	} else {
		m_firstIteration = false;

		if(this->numRows() > 0) {
			return true;
		} else {
			return false;
		}
	}
}

size_t database_t::iterator_t::numRows() {
	if(m_queryResult != NULL) {
		return static_cast<size_t>(mysql_num_rows(m_queryResult));
	} else {
		return 0;
	}
}

size_t database_t::iterator_t::numFields() {
	if(m_queryResult != NULL) {
		return mysql_num_fields(m_queryResult); 
	} else {
		return 0;
	}
}

void database_t::iterator_t::mapFields() {
	MYSQL_FIELD *field;
	size_t i = 0;

	while(field = mysql_fetch_field(m_queryResult)) {
		m_fieldMap[field->name] = i;
		++i;
	}
}