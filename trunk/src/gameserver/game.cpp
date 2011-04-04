#include <boost/bind.hpp>
#include "game.h"

game_t::game_t(eMUCore::logger_t &logger,
				eMUCore::socketContextManager_t<gameServerUser_t> &userManager,
				eMUCore::scheduler_t &scheduler,
				protocol_t &protocol,
				dataServerProtocol_t &dataServerProtocol,
				versionConfiguration_t &versionConfiguration,
				const disconnectCallback_t &disconnectCallback):
  m_monsterManager(m_monsterAttributesManager,
					userManager.getCount()),
  m_logger(logger),
  m_scheduler(scheduler),
  m_userManager(userManager),
  m_protocol(protocol),
  m_dataServerProtocol(dataServerProtocol),
  m_versionConfiguration(versionConfiguration),
  m_viewportManager(m_protocol),
  m_disconnectCallback(disconnectCallback) {}

void game_t::startup() {
	m_gameConfiguration.read("..\\data\\game.xml");
	m_mapManager.startup("..\\data\\worlds.xml");
	m_gateManager.startup("..\\data\\gates.xml");
	m_monsterAttributesManager.startup("..\\data\\monstersAttributes.xml");
	m_monsterManager.startup("..\\data\\monsters.xml",
								boost::bind(&viewportManager_t::registerObject, &m_viewportManager, _1));
	m_itemAttributesManager.startup("..\\data\\items.xml");
	m_scheduler.insert(eMUCore::schedule_e::_nonSynchronized,
						boost::bind(&game_t::checkSelfClose, this),
						1);
}

void game_t::cleanup() {
	m_monsterManager.cleanup();
	m_gateManager.cleanup();
	m_mapManager.cleanup();
}

void game_t::onLoginRequest(gameServerUser_t &user,
							const std::string &accountId,
							const std::string &password,
							const std::string &clientExeVersion,
							const std::string &clientExeSerial) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Login request :: account [" << accountId << "].";

	if(!user.isLoggedIn()) {
		if(clientExeVersion == m_versionConfiguration.m_versionProtocol
			&& clientExeSerial == m_versionConfiguration.m_serial) {
			m_logger.out();

			m_dataServerProtocol.sendAccountCheckRequest(user.getConnectionStamp(), 
															accountId,
															password,
															user.getIpAddress());
		} else {
			m_logger.append() << " Invalid version or serial [" << clientExeVersion << "][" << clientExeSerial << "].";
			m_logger.out();

			m_protocol.sendLoginAnswer(user, accountCheckResult_e::_invalidVersionOrSerial);
		}
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " User already logged in.";
		m_logger.out();
		m_disconnectCallback(user);
	}
}

void game_t::onAccountCheckAnswer(unsigned int connectionStamp,
							const std::string &accountId,
							accountCheckResult_e::type_t result) {
	gameServerUser_t &user = m_userManager.find(connectionStamp);

	m_logger.in(eMUCore::loggerMessage_e::_info) << user << "[" << accountId << "]";

	if(result == accountCheckResult_e::_success) {
			m_logger.append() << " logged in.";
			m_logger.out();

			user.setAccountId(accountId);
			user.setLoggedIn();
	} else {
		switch(result) {
		case accountCheckResult_e::_invalidPassword:
			m_logger.append() << " Invalid password.";
			break;

		case accountCheckResult_e::_notExists:
			m_logger.append() << " Not exists.";
			break;

		case accountCheckResult_e::_alreadyInUse:
			m_logger.append() << " Already in use.";
			break;

		case accountCheckResult_e::_banned:
			m_logger.append() << " Banned.";
			break;
		}

		m_logger.out();

		user.incerementLoginAttempts();

		if(user.getLoginAttempts() >= 3) {
			result = accountCheckResult_e::_checkAttemptsExceeded; // to many attempts.
		}
	}

	m_protocol.sendLoginAnswer(user, result);
}

void game_t::onCharacterListRequest(gameServerUser_t &user) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Character list request.";
	m_logger.out();

	if(user.isLoggedIn()) {
		m_dataServerProtocol.sendCharacterListRequest(user.getConnectionStamp(), user.getAccountId());
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " User is not logged on account.";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onCharacterListAnswer(unsigned int connectionStamp,
									const eMUShared::characterList_t &characterList) {
	gameServerUser_t &user = m_userManager.find(connectionStamp);

	user.mapCharacterList(characterList);
	user.setAvailableRaces(characterList, m_gameConfiguration.m_advancedRaceLevel);

	m_protocol.sendCharacterListAnswer(user, user.getAvailableRaces(), characterList);
}

void game_t::onClientClose(gameServerUser_t &user) {
	if(user.getCharacter().isActive()) {
		this->onCharacterLeave(user);
	}

	if(user.isLoggedIn()) {
		m_dataServerProtocol.sendLogoutRequest(user.getAccountId());
	}
}

void game_t::onCharacterLeave(gameServerUser_t &user) {
	m_mapManager[user.getCharacter().getMapId()].clearStand(user.getCharacter().getPosition());
	this->saveCharacter(user);
	user.getCharacter().setInactive();

	m_viewportManager.clear(user.getCharacter());
	m_viewportManager.unregisterObject(&user.getCharacter());
}

void game_t::onLogoutRequest(gameServerUser_t &user, clientCloseReason_e::type_t closeReason) {
	user.setCloseReason(closeReason);
	user.setTimeToClose(5);
}

void game_t::onCharacterCreateRequest(gameServerUser_t &user,
										const std::string &name,
										eMUShared::characterRace_e::type_t race) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Create character request :: name [" << name 
													<< "] race [" << race << "].";
	m_logger.out();

	if(user.isLoggedIn()) {
		if(race == eMUShared::characterRace_e::_darkWizard
			|| race == eMUShared::characterRace_e::_darkKnight
			|| race == eMUShared::characterRace_e::_elf
			|| race == eMUShared::characterRace_e::_magicGladiator
			|| race == eMUShared::characterRace_e::_darkLord) {
			m_dataServerProtocol.sendCharacterCreateRequest(user.getConnectionStamp(),
															user.getAccountId(),
															name,
															race);
		} else {
			m_logger.in(eMUCore::loggerMessage_e::_error) << user << " Invalid race.";
			m_logger.out();
			
			m_disconnectCallback(user);
		}
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " User is not logged on account.";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onCharacterCreateAnswer(unsigned int connectionStamp,
										const std::string &name,
										eMUShared::characterRace_e::type_t race,
										characterCreateResult_e::type_t result) {
	gameServerUser_t &user = m_userManager.find(connectionStamp);

	if(result != characterCreateResult_e::_toManyCharacters) {
		m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Character [" << name << "]";

		int slot = 0;

		if(result == characterCreateResult_e::_alreadyExists) {
			m_logger.append() << " already exists.";
		} else if(result == characterCreateResult_e::_success) {
			slot = user.insertToCharacterList(name);
			m_logger.append() << " created at slot [" << slot << "].";
		}

		m_logger.out();

		m_protocol.sendCharacterCreteAnswer(user, result, name, slot, 1, race);
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " already have 5 characters.";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onCharacterDeleteRequest(gameServerUser_t &user,
										const std::string &name,
										const std::string &pin) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Delete character request ::" << " name [" << name << "].";
	m_logger.out();

	if(user.isLoggedIn()) {
		m_dataServerProtocol.sendCharacterDeleteRequest(user.getConnectionStamp(), user.getAccountId(), name, pin);
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " User is not logged on account.";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onCharacterDeleteAnswer(unsigned int connectionStamp,
										const std::string &name,
										characterDeleteResult_e::type_t result) {
	gameServerUser_t &user = m_userManager.find(connectionStamp);

	if(result != characterDeleteResult_e::_noAccountAssociation) {
		m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Character [" << name << "]";

		if(result == characterDeleteResult_e::_success) {
			m_logger.append() << " deleted.";
			user.deleteFromCharacterList(name);
		} else if(result == characterDeleteResult_e::_invalidPin) {
			m_logger.append() << " invalid PIN.";
		}

		m_logger.out();

		m_protocol.sendCharacterDeleteAnswer(user, result);
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " Character [" << name << "]"
														<< " not associated with account.";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onCharacterSelectRequest(gameServerUser_t &user,
									  const std::string &name) {
	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Select character request ::" << " name [" << name << "].";
	m_logger.out();

	if(!user.getCharacter().isActive()) {
		m_dataServerProtocol.sendCharacterSelectRequest(user.getConnectionStamp(), user.getAccountId(), name);
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " User is playing -> character " << user.getCharacter() << ".";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onCharacterSelectAnswer(unsigned int connectionStamp,
									 const eMUShared::characterAttributes_t &attr) {
	gameServerUser_t &user = m_userManager.find(connectionStamp);

	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Preparing character [" << attr.m_name << "].";
	m_logger.out();

	if(m_mapManager[attr.m_mapId].canStand(attr.m_position)) {
		character_t &character = user.getCharacter();

		m_mapManager[attr.m_mapId].setStand(attr.m_position);

		character.setAttributes(attr);
		character.setPreview();
		character.setActive();
		m_protocol.sendCharacterSelectAnswer(user, character);
		m_protocol.sendTextNotice(user, m_gameConfiguration.m_welcomeNotice);

		character.activateTeleportEffect();
		m_viewportManager.registerObject(&character);
		m_viewportManager.generate(character);
		character.deactivateTeleportEffect();
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " Invalid start coordinates " << attr.m_position << ".";
		m_logger.out();

		//m_disconnectCallback(user);
	}
}

void game_t::onCharacterMoveRequest(gameServerUser_t &user,
									const eMUShared::position_t &pos,
									unsigned char direction,
									const map_t::path_t &path) {
	character_t &character = user.getCharacter();

	if(GetTickCount() - character.getLastMoveTime() >= 100) {
		if(m_mapManager[character.getMapId()].isPathValid(path)) {
			m_mapManager[character.getMapId()].resetStand(character.getPosition(), pos);

			character.getAttributes().m_position = pos;
			character.setDirection(direction);
			character.setLastMoveTime(GetTickCount());
			character.setPose(characterAction_e::_setStand);  // default - stand.

			m_viewportManager.update(character);

			m_protocol.sendViewportObjectMoveRequest(character);
		} else {
			m_logger.in(eMUCore::loggerMessage_e::_error) << user << " " << character << " Invalid path for character.";
			m_logger.out();

			#ifdef _DEBUG
			m_logger.in(eMUCore::loggerMessage_e::_debug) << user << " " << character << " Path dump: " 
															<< m_mapManager[character.getMapId()].dumpPath(path) << ".";
			m_logger.out();
			#endif

			m_disconnectCallback(user);
		}
	}
}

void game_t::onCharacterTeleportRequest(gameServerUser_t &user,
										unsigned short gateId) {
	character_t &character = user.getCharacter();

	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " " << character << " Requested teleport to gate [" << gateId << "].";
	m_logger.out();

	gate_t &gate = m_gateManager[gateId];

	if(gate.isInGate(character.getAttributes().m_position)) {
		if(gate.getRequiredLevel() <= character.getAttributes().m_level) {
			gate_t &destGate = m_gateManager[gate.getDestId()];
			eMUShared::position_t pos = m_mapManager[destGate.getMapId()].getRandomPosition(destGate.getStartPosition(),
																								destGate.getEndPosition());

			#ifdef _DEBUG
			m_logger.in(eMUCore::loggerMessage_e::_debug) << "Source gate: " << gate << ", destination gate: " << destGate << ".";
			m_logger.out();
			#endif

			this->teleportCharacter(user,
									destGate.getMapId(),
									pos,
									destGate.getDirection(),
									destGate.getId());
		} else {
			m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Character level not match to enter gate.";
			m_logger.out();

			m_disconnectCallback(user);
		}
	} else {
		m_logger.in(eMUCore::loggerMessage_e::_error) << user << " Character is not in gate range.";
		m_logger.out();

		m_disconnectCallback(user);
	}
}

void game_t::onPublicChat(gameServerUser_t& user,
						  const std::string &message) {
	if(message[0] != '/') {
		m_protocol.sendPublicChatAnswer(user, message);
	}
}

void game_t::onCharacterAction(gameServerUser_t &user,
							   unsigned char direction,
							   characterAction_e::type_t actionId) {
	user.getCharacter().setDirection(direction);
	user.getCharacter().setPose(actionId);
	m_protocol.sendViewportCharacterActionRequest(user.getCharacter(), actionId, user.getIndex());
}

void game_t::onWhisperChatRequest(gameServerUser_t& user,
								  const std::string &receiverName,
								  const std::string &message) {
	gameServerUser_t *receiver = NULL;

	try {
		receiver = &m_userManager.find(receiverName);
	} catch(eMUCore::exception_t&) {
		m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Whisper chat with [" << receiverName << "] is currenty unavailable.";
		m_logger.out();

		m_protocol.sendTextNotice(user, "User is currenty unavailable.", gameNotice_e::_blue);
		return;
	}

	if(receiver) {
		m_protocol.sendWhisperChatAnswer(*receiver, user.getCharacter().getName(), message);
	}
}

void game_t::onQueryExceptionNotice(unsigned int connectionStamp,
								const std::string &what) {
	gameServerUser_t &user = m_userManager.find(connectionStamp);

	m_logger.in(eMUCore::loggerMessage_e::_error) << "DataServer query error: " << user << " " << what;
	m_logger.out();

	//m_disconnectCallback(user);
}

void game_t::checkSelfClose() {
	for(size_t i = 0; i < m_userManager.getCount(); ++i) {
		gameServerUser_t &user = m_userManager[i];

		if(user.isLoggedIn()) {
			if(user.getCloseReason() != clientCloseReason_e::_none) {
				if(user.getTimeToClose() == 0) {
					if(user.getCloseReason() == clientCloseReason_e::_switchCharacter && user.getCharacter().isActive()) {
						this->onCharacterLeave(user);
					}

					m_protocol.sendLogoutAnswer(user, user.getCloseReason());

					user.resetCloseReason();
				} else {
					std::stringstream notice;
					notice << "You will left the game after " << user.getTimeToClose() << " seconds.";
					m_protocol.sendTextNotice(user, notice.str(), gameNotice_e::_blue); // 1 - blue, 0 - gold

					user.decrecemntTimeToClose();
				}
			}
		}
	}
}

void game_t::saveCharacter(gameServerUser_t &user) const {
	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " Saving character :: name " << user.getCharacter() << ".";
	m_logger.out();

	m_dataServerProtocol.sendCharacterSaveRequest(user.getAccountId(),
													user.getCharacter().getAttributes());
}

void game_t::teleportCharacter(gameServerUser_t &user,
								unsigned char mapId,
								const eMUShared::position_t &pos,
								unsigned char direction,
								unsigned char gateId) {
	character_t &character = user.getCharacter();

	m_logger.in(eMUCore::loggerMessage_e::_info) << user << " " << character << " Teleporting to"
													<< " [" << static_cast<int>(mapId) << "]" << pos << ".";
	m_logger.out();

	m_mapManager[character.getMapId()].clearStand(character.getAttributes().m_position);
	m_mapManager[mapId].setStand(pos);

	m_viewportManager.clear(character);

	character.setMapId(mapId);
	character.getAttributes().m_position = pos;
	character.setDirection(direction);
	character.setPose(characterAction_e::_setStand); // default - stand.
	m_protocol.sendCharacterTeleportAnswer(user, mapId, pos, direction, gateId);

	character.activateTeleportEffect();
	m_viewportManager.generate(character);
	character.deactivateTeleportEffect();
}