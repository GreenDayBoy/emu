#include <iostream>
#include <signal.h>
#include "dataserver.h"

#pragma warning(disable: 4355)

bool dataServer_t::m_interrupt = false;

dataServer_t::dataServer_t(size_t maxStoredLogsCount,
							size_t userMax,
							unsigned short port):
  m_userManager(userMax,
				boost::bind(&dataServer_t::onContextAttach, this, _1),
				boost::bind(&dataServer_t::onContextReceive, this, _1),
				boost::bind(&dataServer_t::onContextClose, this, _1)),
				m_logger("log\\logs.html",
				maxStoredLogsCount),
  m_scheduler(m_synchronizer),
				m_iocpEngine(m_logger,
				m_synchronizer),
				m_tcpServer(m_logger,
				m_iocpEngine,
				boost::bind(&dataServer_t::onContextAllocate, this),
				port),
  m_protocol(m_game,
				boost::bind(&dataServer_t::send, this, _1, _2)),
				m_game(m_logger,
				m_scheduler,
				m_protocol,
				boost::bind(&dataServer_t::disconnect, this, _1)),
  m_userCount(0) {}

dataServer_t::~dataServer_t() {
	this->disconnectAll();
}

void dataServer_t::startup() {
	m_logger.startup();

	m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << "Initializing allowed host list.";
	m_logger.out();
	m_allowedHostList.read("allowedHostList.xml");

	m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << "Initializing game.";
	m_logger.out();
	m_game.startup();

	m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << "Starting iocp engine.";
	m_logger.out();
	m_iocpEngine.startup();

	m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << "Starting tcp server on port " << m_tcpServer.getListenPort() << ".";
	m_logger.out();
	m_tcpServer.startup();

	this->updateWindowTitle();

	m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << "Waiting for connections.";
	m_logger.out();
}

void dataServer_t::send(dataServerUser_t &user, const eMUCore::packet_t &packet) {
	#ifdef _DEBUG
	m_logger.in(eMUCore::logger_t::_MESSAGE_PROTOCOL) << user << " Sending " << packet << ".";
	m_logger.out();
	#endif

	m_iocpEngine.write(user, packet.getData(), packet.getSize());
}

void dataServer_t::worker() {
	while(!m_interrupt) {
		m_scheduler.worker();
		Sleep(1);
	}
}

void dataServer_t::updateWindowTitle() const {
	std::stringstream titleStream;
	titleStream << "[DataServer] ::"
				<< " [Users: " << m_userCount << "/" << m_userManager.getCount() << "] ::"
				<< " [Port: " << m_tcpServer.getListenPort() << "]";

	SetConsoleTitle(titleStream.str().c_str());
}

void dataServer_t::disconnectAll() {
	for(size_t i = 0; i < m_userManager.getCount(); ++i) {
		if(m_userManager[i].isActive()) {
			this->disconnect(m_userManager[i]);
		}
	}
}

dataServerUser_t* dataServer_t::onContextAllocate() {
	dataServerUser_t *user = m_userManager.findFree();

	if(user == NULL) {
		m_logger.in(eMUCore::logger_t::_MESSAGE_ERROR) << "No free objects found.";
		m_logger.out();
	}

	return user;
}

void dataServer_t::onContextAttach(eMUCore::socketContext_t &context) {
	dataServerUser_t &user = reinterpret_cast<dataServerUser_t&>(context);

	try {
		m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << user << " Connected.";
		m_logger.out();

		// -----------------------
		++m_userCount;
		this->updateWindowTitle();
		// -----------------------

		if(!m_allowedHostList.isHostAllowed(user.getIpAddress())) {
			m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << user << " Host is not allowed to connect.";
			m_logger.out();
			this->disconnect(user);
		}
	} catch(eMUCore::exception_t &e) {
		m_logger.in(eMUCore::logger_t::_MESSAGE_ERROR) << "Exception: " << user << " " << e.what();
		m_logger.out();
		this->disconnect(user);
	} catch(std::exception &e) {
		m_logger.in(eMUCore::logger_t::_MESSAGE_ERROR) << "std exception: " << user << " " << e.what();
		m_logger.out();
		this->disconnect(user);
	} catch(...) {
		m_logger.in(eMUCore::logger_t::_MESSAGE_ERROR) << "Exception: " << user << " Unknown exception.";
		m_logger.out();
		this->disconnect(user);
	}
}

void dataServer_t::onContextReceive(eMUCore::socketContext_t &context) {
	dataServerUser_t &user = reinterpret_cast<dataServerUser_t&>(context);

	try {
		size_t parsedDataSize = 0;

		do {
			size_t rawDataSize = eMUCore::packet_t::getRawDataSize(&user.getRecvBuffer().m_data[parsedDataSize]);
			eMUCore::packet_t packet(&user.getRecvBuffer().m_data[parsedDataSize]);

			#ifdef _DEBUG
			m_logger.in(eMUCore::logger_t::_MESSAGE_PROTOCOL) << user << " Received " << packet << ".";
			m_logger.out();
			#endif

			// -------------------------------
			m_protocol.core(user, packet);
			// -------------------------------

			parsedDataSize += rawDataSize;
		} while(parsedDataSize < user.getRecvBuffer().m_dataSize);
	} catch(eMUCore::exception_t &e) {
		m_logger.in(eMUCore::logger_t::_MESSAGE_ERROR) << "Exception: " << user << " " << e.what();
		m_logger.out();
		this->disconnect(user);
	} catch(std::exception &e) {
		m_logger.in(eMUCore::logger_t::_MESSAGE_ERROR) << "std exception: " << user << " " << e.what();
		m_logger.out();
		this->disconnect(user);
	} catch(...) {
		m_logger.in(eMUCore::logger_t::_MESSAGE_ERROR) << "Exception: " << user << " Unknown exception.";
		m_logger.out();
		this->disconnect(user);
	}
}

void dataServer_t::onContextClose(eMUCore::socketContext_t &context) {
	dataServerUser_t &user = reinterpret_cast<dataServerUser_t&>(context);

	m_logger.in(eMUCore::logger_t::_MESSAGE_INFO) << user << " Disconnected.";
	m_logger.out();

	// -----------------------
	--m_userCount;
	this->updateWindowTitle();
	// -----------------------
}

void dataServer_t::fatalHandler(int /*signalId*/) {
	//eMUCore::stackTrace_t::dumpCallTree("log\\stackTrace.txt");
}

DWORD WINAPI dataServer_t::consoleEventHandler(unsigned int /*eventId*/) {
	if(!dataServer_t::m_interrupt) {
		int ret = MessageBox(0, "Do you really want to quit the server?", "eMU::DataServer", MB_ICONQUESTION | MB_YESNO);
		if(ret == IDYES) {
			dataServer_t::stop();
		}
	}

    return TRUE;
}

void main(int argsCount, char *argsVect[]) {
	signal(SIGABRT, dataServer_t::fatalHandler);
	signal(SIGFPE, dataServer_t::fatalHandler);
	signal(SIGILL, dataServer_t::fatalHandler);
	signal(SIGSEGV, dataServer_t::fatalHandler);
	signal(SIGTERM, dataServer_t::fatalHandler);

	//system("COLOR 0F");
	std::cout << "		+-----------------------------------------------+" << std::endl;
	std::cout << "		|						|" << std::endl;
	std::cout << "		|		eMU :: DataServer		|"  << std::endl;
	std::cout << "		|		 Build: " << __DATE__ << "		|" << std::endl;
	std::cout << "		|						|" << std::endl;
	std::cout << "		+-----------------------------------------------+" << std::endl << std::endl;

	try {
		if(SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(dataServer_t::consoleEventHandler), TRUE) != 0) {
			WSADATA wsaData = {0};
			if(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
				if(argsCount == 4) {
					dataServer_t *dataServer = new dataServer_t(boost::lexical_cast<size_t>(argsVect[1]),
																boost::lexical_cast<size_t>(argsVect[2]),
																boost::lexical_cast<unsigned short>(argsVect[3]));
					dataServer->startup();
					dataServer->worker();
					delete dataServer;
				} else {
					eMUCore::exception_t e;
					e.in() << __FILE__ << ":" << __LINE__ << "[main()] Invalid command line.";
					throw e;
				}
			} else {
				eMUCore::exception_t e;
				e.in() << __FILE__ << ":" << __LINE__ << "[main()] WSAStartup failed with error#" << WSAGetLastError() << ".";
				throw e;
			}
		} else {
			eMUCore::exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << "[main()] SetConsoleCtrlHandler failed with error#" << GetLastError() << ".";
			throw e;
		}
	} catch(eMUCore::exception_t &e) {
		std::cout << "[Exception] " << e.what() << std::endl;
	} catch(std::exception &e) {
		std::cout << "[std exception] " << e.what() << std::endl;
	}

	system("PAUSE");
}

#pragma warning(default: 4355)