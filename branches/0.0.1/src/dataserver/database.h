#ifndef eMU_DATASERVER_DATABASE_H
#define eMU_DATASERVER_DATABASE_H

#include <string>
#include <sstream>
#include <winsock2.h> // What the fuck?????????????????
#include <mysql\mysql.h>
#include "..\core\core.h"

class database_t {
public:
	database_t(eMUCore::logger_t &logger);

	void startup(const std::string &hostname,
					unsigned short port,
					const std::string &dbName,
					const std::string &userName,
					const std::string &password);
	void cleanup();

	inline std::stringstream& query() { 
		m_queryBuffer.str("");
		return m_queryBuffer; 
	}

	void execute();
	void ping();
	MYSQL_RES* getQueryResult();

	class iterator_t {
	public:
		iterator_t(MYSQL_RES *queryResult);
		~iterator_t();

		bool nextRow();
		size_t getNumRows();
		size_t getNumFields();

		inline bool empty() { return this->getNumRows() == 0; }

		template<typename T>
		T getValue(const std::string &fieldName) {
			fieldMap_t::iterator iter = m_fieldMap.find(fieldName);

			if(iter != m_fieldMap.end()) {
				return getValue<T>(iter->second);
			} else {
				eMUCore::exception_t e;
				e.in() << "Field '" << fieldName << "' is not defined.";
				throw e;
			}
		}

		template<typename T>
		T getValue(size_t index) {
			if(this->getNumFields() > index) {
				if(m_resultRow[index] == NULL) {
					return boost::lexical_cast<T>(0);
				} else {
					return boost::lexical_cast<T>(m_resultRow[index]);
				}
			} else {
				eMUCore::exception_t e;
				e.in() << "Field index " << index << " out of range.";
				throw e;
			}
		}

	private:
		typedef std::map<std::string, size_t> fieldMap_t;

		iterator_t();
		iterator_t(const iterator_t&);
		iterator_t& operator=(const iterator_t&);

		void mapFields();

		fieldMap_t	m_fieldMap;
		MYSQL_RES	*m_queryResult;
		MYSQL_ROW	m_resultRow;
		bool		m_firstIteration;
	};

private:
	database_t();
	database_t(const database_t &database);
	database_t& operator=(const database_t &database);

	std::stringstream m_queryBuffer;
	MYSQL m_connectionHandle;
	eMUCore::logger_t &m_logger;
};

#endif // eMU_DATASERVER_DATABASE_H