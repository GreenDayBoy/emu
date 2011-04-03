#include <boost/bind.hpp>
#include "game.h"

game_t::game_t(eMUCore::logger_t &logger,
				eMUCore::scheduler_t &scheduler,
				protocol_t &protocol,
				const disconnectCallback_t &disconnectCallback):
  m_logger(logger),
  m_scheduler(scheduler),
  m_database(logger),
  m_protocol(protocol),
  m_disconnectCallback(disconnectCallback) {}

void game_t::startup() {
	m_configuration.read("configuration.xml");

	m_database.startup(m_configuration.m_dbHostname,
						m_configuration.m_dbPort,
						m_configuration.m_dbName,
						m_configuration.m_dbUserName,
						m_configuration.m_dbPassword);

	m_scheduler.insert(eMUCore::schedule_e::_nonSynchronized,
						boost::bind(&database_t::ping, &m_database),
						300);
}

void game_t::cleanup() {
	m_database.cleanup();
}

void game_t::onAccountCheckRequest(dataServerUser_t &user,
									unsigned int connectionStamp,
									const std::string &accountId,
									const std::string &password,
									const std::string &ipAddress) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << "Account check request :: account [" << accountId << "] ipAddress [" << ipAddress << "].";
	m_logger.out();

	try {
		m_database.query() << "SELECT"
								<< " `eMU_AccountCheck`("
									<< "'" << accountId << "'"
									<< ", '" << password << "'"
									<< ", '" << ipAddress << "');";
		m_database.execute();
		database_t::iterator_t iter = m_database.getQueryResult();

		int checkResult = iter.getValue<int>(0);

		m_protocol.sendAccountCheckAnswer(user,
											connectionStamp,
											accountId,
											checkResult);
	} catch(eMUCore::exception_t &e) {
		this->onQueryExceptionNotice(user, connectionStamp, e.what());
	}
}

void game_t::onCharacterListRequest(dataServerUser_t &user,
										unsigned int connectionStamp,
										const std::string &accountId) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << "Character list request :: account [" << accountId << "].";
	m_logger.out();

	try {
		m_database.query() << "SELECT"
								<< " `name`, `race`, `level`, `controlCode`"
							<< " FROM"
								<< " `characters`"
							<< " WHERE"
								<< " `accountId` = '" << accountId << "'"
							<< " ORDER BY"
								<< " `created`"
							<< " ASC;";
		m_database.execute();
		database_t::iterator_t iter = m_database.getQueryResult();

		eMUShared::characterList_t characterList;

		while(iter.nextRow()) {
			eMUShared::characterListAttributes_t character;
			character.m_name = iter.getValue<std::string>("name");
			character.m_race = static_cast<eMUShared::characterRace_e::type_t>(iter.getValue<unsigned int>("race"));
			character.m_level = iter.getValue<unsigned short>("level");
			character.m_controlCode = iter.getValue<unsigned int>("controlCode");

			characterList.push_back(character);
		}

		m_protocol.sendCharacterListAnswer(user, connectionStamp, characterList);
	} catch(eMUCore::exception_t &e) {
		this->onQueryExceptionNotice(user, connectionStamp, e.what());
	}
}

void game_t::onLogoutRequest(dataServerUser_t &/*user*/,
								const std::string &accountId) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << "Logout request :: account [" << accountId << "].";
	m_logger.out();

	try {
		m_database.query() << "UPDATE"
								<< " `accounts`"
							<< " SET"
								<< " `status` = 0,"
								<< " `logoutDate` = NOW()"
							<< " WHERE"
								<< " `id` = '" << accountId << "';";
		m_database.execute();
	} catch(eMUCore::exception_t &e) {
		m_logger.in(eMUCore::loggerMessage_e::_error) << e.what();
		m_logger.out();
	}
}

void game_t::onCharacterCreateRequest(dataServerUser_t &user,
										unsigned int connectionStamp,
										const std::string &accountId,
										const std::string &name,
										eMUShared::characterRace_e::type_t race) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << "Character create request :: account [" << accountId << "] name [" << name 
													<< "] race [" << race << "].";
	m_logger.out();

	try {
		m_database.query() << "SELECT"
								<< " `eMU_CharacterCreate`("
									<< "'" << accountId << "'"
									<< ", '" << name << "'"
									<< ", '" << race << "');";
		m_database.execute();
		database_t::iterator_t iter = m_database.getQueryResult();

		int createResult = iter.getValue<int>(0);

		m_protocol.sendCharacterCreateAnswer(user,
												connectionStamp,
												name,
												race,
												createResult);
	} catch(eMUCore::exception_t &e) {
		this->onQueryExceptionNotice(user, connectionStamp, e.what());
	}
}

void game_t::onCharacterDeleteRequest(dataServerUser_t &user,
										unsigned int connectionStamp,
										const std::string &accountId,
										const std::string &name,
										const std::string &pin) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << "Character delete request :: account [" << accountId << "] name [" << name << "].";
	m_logger.out();

	try {
		m_database.query() << "SELECT"
								<< " `eMU_CharacterDelete`("
									<< "'" << accountId << "'"
									<< ", '" << name << "'"
									<< ", '" << pin << "');";
		m_database.execute();
		database_t::iterator_t iter = m_database.getQueryResult();

		int deleteResult = iter.getValue<int>(0);

		m_protocol.sendCharacterDeleteAnswer(user,
												connectionStamp,
												name,
												deleteResult);
	} catch(eMUCore::exception_t &e) {
		this->onQueryExceptionNotice(user, connectionStamp, e.what());
	}
}

void game_t::onCharacterSelectRequest(dataServerUser_t &user,
										unsigned int connectionStamp,
										const std::string &accountId,
										const std::string &name) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << "Character select request :: account [" << accountId << "] name [" << name << "].";
	m_logger.out();

	try {
		m_database.query() << "SELECT"
								<< " `race`,"
								<< " `posX`,"
								<< " `posY`,"
								<< " `mapId`,"
								<< " `direction`,"
								<< " `experience`,"
								<< " `levelUpPoints`,"
								<< " `level`,"
								<< " `strength`,"
								<< " `agility`,"
								<< " `vitality`,"
								<< " `energy`,"
								<< " `command`,"
								<< " `health`,"
								<< " `maxHealth`,"
								<< " `mana`,"
								<< " `maxMana`,"
								<< " `money`,"
								<< " `controlCode`"
							<< " FROM"
								<< " `characters`"
							<< " WHERE"
								<< " `accountId` = '" << accountId << "'"
							<< " AND"
								<< " `name` = '" << name << "';";

		m_database.execute();
		database_t::iterator_t iter = m_database.getQueryResult();

		eMUShared::characterAttributes_t attr;

		attr.m_name = name;
		attr.m_race = static_cast<eMUShared::characterRace_e::type_t>(iter.getValue<unsigned int>("race"));

		attr.m_strength = iter.getValue<unsigned short>("strength");
		attr.m_agility = iter.getValue<unsigned short>("agility");
		attr.m_vitality = iter.getValue<unsigned short>("vitality");
		attr.m_energy = iter.getValue<unsigned short>("energy");
		attr.m_command = iter.getValue<unsigned short>("command");

		attr.m_mapId = iter.getValue<int>("mapId");
		attr.m_posX = iter.getValue<int>("posX");
		attr.m_posY = iter.getValue<int>("posY");
		attr.m_direction = iter.getValue<int>("direction");

		attr.m_health = iter.getValue<unsigned short>("health");
		attr.m_maxHealth = iter.getValue<unsigned short>("maxHealth");
		attr.m_mana = iter.getValue<unsigned short>("mana");
		attr.m_maxMana = iter.getValue<unsigned short>("maxMana");

		attr.m_experience = iter.getValue<unsigned int>("experience");

		attr.m_controlCode = iter.getValue<int>("controlCode");
		attr.m_levelUpPoints = iter.getValue<unsigned short>("levelUpPoints");
		attr.m_level = iter.getValue<unsigned short>("level");
		attr.m_money = iter.getValue<unsigned int>("money");
		
		// -------------------------
		// Temporary.
		attr.m_addPoints = 0;
		attr.m_maxAddPoints = 0;
		
		attr.m_minusPoints = 0;	
		attr.m_maxMinusPoints = 0;

		attr.m_shield = 0;
		attr.m_maxShield = 0;

		attr.m_stamina = 0;
		attr.m_maxStamina = 0;
	
		attr.m_pkLevel = 0;
		// -------------------------

		m_protocol.sendCharacterSelectAnswer(user, connectionStamp, attr);
	} catch(eMUCore::exception_t &e) {
		this->onQueryExceptionNotice(user, connectionStamp, e.what());
	}
}

void game_t::onCharacterSaveRequest(dataServerUser_t &user,
									const std::string &accountId,
									const eMUShared::characterAttributes_t &attr) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << "Character save request :: account [" << accountId << "] name [" << attr.m_name << "].";
	m_logger.out();

	try {
		m_database.query() << "UPDATE"
								<< " `characters`"
							<< " SET"
								<< " `race` = " << attr.m_race
								<< ", `posX` = "  << static_cast<unsigned int>(attr.m_posX)
								<< ", `posY` = "  << static_cast<unsigned int>(attr.m_posY)
								<< ", `mapId` = " << static_cast<unsigned int>(attr.m_mapId)
								<< ", `direction` = " << static_cast<unsigned int>(attr.m_direction)
								<< ", `experience` = "  << attr.m_experience
								<< ", `levelUpPoints` = " << attr.m_levelUpPoints
								<< ", `level` = " << attr.m_level
								<< ", `strength` = " << attr.m_strength
								<< ", `agility` = " << attr.m_agility
								<< ", `vitality` = " << attr.m_vitality
								<< ", `energy` = " << attr.m_energy
								<< ", `command` = " << attr.m_command
								<< ", `health` = " << attr.m_health
								<< ", `maxHealth` = " << attr.m_maxHealth
								<< ", `mana` = " << attr.m_mana
								<< ", `maxMana` = " << attr.m_maxMana
								<< ", `money` = " << attr.m_money
								<< ", `controlCode` = " << static_cast<unsigned int>(attr.m_controlCode)
							<< " WHERE"
								<< " `accountId` = '" << accountId << "'"
							<< " AND"
								<< " `name` = '" << attr.m_name << "';";

		m_database.execute();
	} catch(eMUCore::exception_t &e) {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " " << e.what();
		m_logger.out();
	}
}

void game_t::onQueryExceptionNotice(dataServerUser_t &user,
							  unsigned int connectionStamp,
							  const std::string &what) {
	m_logger.in(eMUCore::loggerMessage_e::_error) << user << " " << what;
	m_logger.out();

	m_protocol.sendQueryExceptionNotice(user, connectionStamp, what);
}