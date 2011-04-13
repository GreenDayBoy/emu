#include <iomanip>
#include <iostream>
#include "core.h"

using namespace eMUCore;

const std::string eMUCore::logger_t::c_loggerMessageHeader[]	= {"[Debug] ",
																	"[Info] ",
																	"[Warning] ",
																	"[Error] ",
																	"[Protocol] "};

const std::string eMUCore::logger_t::c_loggerMessageCSSClass[] = {"msg_debug",
																	"msg_information",
																	"msg_warning",
																	"msg_error",
																	"msg_protocol"};

const std::string eMUCore::logger_t::c_loggerMessageIconPath[] = {"images/icon_debug.png",
																	"images/icon_information.png",
																	"images/icon_warning.png",
																	"images/icon_error.png",
																	"images/icon_protocol.png"};

const unsigned char eMUCore::logger_t::c_loggerMessageColor[] = {11,
																	2,
																	14,
																	12,
																	5};

logger_t::logger_t(const std::string &fileName, size_t maxStoredLogsCount):
  m_fileName(fileName),
  m_file(NULL),
  m_tableNode(NULL),
  m_maxStoredLogCount(maxStoredLogsCount),
  m_storedLogCount(0),
  m_stdOutput(INVALID_HANDLE_VALUE) {}

void logger_t::startup() {
	m_file = xmlReadFile(m_fileName.c_str(), 0, XML_PARSE_NOBLANKS);

	if(m_file != NULL) {
		xmlNodePtr htmlNode = xmlDocGetRootElement(m_file);

		if(htmlNode != NULL) {
			m_tableNode = htmlNode->children->next->children;
			m_stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

			if(m_stdOutput == INVALID_HANDLE_VALUE) {
				exception_t e;
				e.in() << __FILE__ << ":" << __LINE__ << " :: Could not get standard output handle."
						<< " GetStdHandle failed with error #" << GetLastError() << ".";
				throw e;
			}
		} else {
			exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << " :: File " << m_fileName << "has invalid format.";
			throw e;
		}
	} else {
		exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << " :: Could not open " << m_fileName << ".";
		throw e;
	}
}

void logger_t::cleanup() {
	if(m_file != NULL)	{
		xmlSaveFile(m_fileName.c_str(), m_file);
		xmlFreeDoc(m_file);
	}
}

std::stringstream& logger_t::in(loggerMessage_e::type_t type) {
	m_synchronizer.lock();

	SYSTEMTIME localTime = {0};
	GetLocalTime(&localTime);

	m_date << "["
				<< std::setfill('0') << std::setw(2) << localTime.wDay
				<< "/"
				<< std::setfill('0') << std::setw(2) << localTime.wMonth
				<< "/"
				<< std::setfill('0') << std::setw(4) << localTime.wYear
				<< " :: "
				<< std::setfill('0') << std::setw(2) << localTime.wHour
				<< ":"
				<< std::setfill('0') << std::setw(2) << localTime.wMinute
				<< ":"
				<< std::setfill('0') << std::setw(2) << localTime.wSecond
				<< "] ";
				//<< c_loggerMessageHeader[logMessageType]
				//<< " ";

	m_currentMessageType = type;
	return m_stream;
}

void logger_t::out() {
	// 1. Utworz wezel <tr></tr> w wezle <table></table> dla nowej wiadomosci.
	xmlNodePtr trMessageNode = xmlNewChild(m_tableNode, 0, reinterpret_cast<const xmlChar*>("tr"), NULL);
	// --------------------------------------------------------------------------------

	// 2. Tworzenie ikony wiadomosci.
	// --------------------------------------------------------------------------------
	// 2.1. Utworz wezel <td></td> w utworzonym wczesniej wezle <tr></tr>.
	//xmlNodePtr tdIconNode = xmlNewChild(trMessageNode, 0, reinterpret_cast<const xmlChar*>("td"), NULL);

	// 2.2. Dodaj atrybut "class = log_icon" do wezla <td></td>.
	//xmlNewProp(tdIconNode, reinterpret_cast<const xmlChar*>("class"), reinterpret_cast<const xmlChar*>("log_icon"));

	// 2.3. Utworz wezel <img /> w utworzonym wczesniej wezle <td></td>.
	//xmlNodePtr imgIconNode = xmlNewChild(tdIconNode, 0, reinterpret_cast<const xmlChar*>("img"), NULL);

	// 2.4. Dodaj atrybut src = sIconPath do wezla <img />
	//xmlNewProp(imgIconNode,
	//			reinterpret_cast<const xmlChar*>("src"),
	//			reinterpret_cast<const xmlChar*>(c_loggerMessageIconPath[m_currentMessageType].c_str()));
	// --------------------------------------------------------------------------------

	std::string message = m_date.str()
							+ c_loggerMessageHeader[m_currentMessageType]
							+ m_stream.str();

	// 3. Utworz wezel <td></td> w utworzonym wczesniej wezle <tr></tr>.
	xmlNodePtr tdMessageNode = xmlNewChild(trMessageNode,
											0,
											reinterpret_cast<const xmlChar*>("td"),
											reinterpret_cast<const xmlChar*>(message.c_str()));

	// 3.1. Dodaj atrybut class = sMessageClass do wezla <td></td>.
	xmlNewProp(tdMessageNode,
				reinterpret_cast<const xmlChar*>("class"),
				reinterpret_cast<const xmlChar*>(c_loggerMessageCSSClass[m_currentMessageType].c_str()));

	std::cout << m_date.str();
	m_date.str("");

	this->color(c_loggerMessageColor[m_currentMessageType]);
	std::cout << c_loggerMessageHeader[m_currentMessageType] << m_stream.str() << std::endl;
	m_stream.str("");

	this->color(7); // Reset color.

	++m_storedLogCount;

	if(m_storedLogCount == m_maxStoredLogCount) {
		m_storedLogCount = 0;
		xmlSaveFile(m_fileName.c_str(), m_file);
	}

	m_synchronizer.unlock();
}