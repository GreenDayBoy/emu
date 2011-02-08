// -----------------------------------------------
//
// Solution name:			eMU
// Project name:			core
// File name:				core.h
// Created:					2010-05-09 18:32
// Author:					f1x
//
// -----------------------------------------------
#ifndef eMU_CORE_CORE_H
#define eMU_CORE_CORE_H

#include <winsock2.h>
#include <windows.h>
#include <libxml/parser.h>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include <iomanip>

#pragma warning(disable: 4251)
#pragma warning(disable: 4290)

#ifdef eMUCORE_EXPORTS
#define eMUCORE_DECLSPEC __declspec(dllexport)
#else
#define eMUCORE_DECLSPEC __declspec(dllimport)
#endif

namespace eMUCore {

template<typename T>
T role(T min, T max) {
	if(max >= min) {
		max -= min;
	} else {
		T tempRange = min - max;
		min = max;
		max = tempRange;
	}

	return static_cast<T>(max ? min + rand() / (RAND_MAX + 1.0) * static_cast<double>(max) : min);
}
	
eMUCORE_DECLSPEC bool isIpAddress(std::string hostname);
eMUCORE_DECLSPEC std::string convertToIpAddress(std::string hostname);

class eMUCORE_DECLSPEC exception_t: public std::exception {
public:
	exception_t() {}

	exception_t(const exception_t &e) { m_stream << e.m_stream.rdbuf(); }

	~exception_t() {}

	inline std::stringstream& in() {
		m_stream.str("");
		return m_stream;
	}

	const char* what() { 
		m_message = m_stream.str();
		return m_message.c_str();
	}

private:
	std::stringstream m_stream;
	std::string m_message;
};

class eMUCORE_DECLSPEC synchronizer_t {
public:
	synchronizer_t() { InitializeCriticalSection(&m_criticalSection); }
	~synchronizer_t() { DeleteCriticalSection(&m_criticalSection); }
	inline void lock() { EnterCriticalSection(&m_criticalSection); }
	inline void unlock() { LeaveCriticalSection(&m_criticalSection); }

private:
	synchronizer_t(const synchronizer_t&);
	synchronizer_t& operator=(const synchronizer_t&);

	CRITICAL_SECTION m_criticalSection;
};

class eMUCORE_DECLSPEC profiler_t {
public:
	profiler_t(const std::string &functionName):
	  m_functionName(functionName) { QueryPerformanceCounter(&m_startTime); }

	~profiler_t() {
		time_t duration = this->getDuration();

		static synchronizer_t synchronizer;
		synchronizer.lock();

		static std::ofstream file("log\\profiles.txt", std::ios::out);

		if(file) {
			file << m_functionName << "() duration: " << duration / 1000 << "ms." << std::endl;
		}

		synchronizer.unlock();
	}

private:
	profiler_t();
	profiler_t(const profiler_t&);
	profiler_t& operator=(const profiler_t&);

	time_t getDuration() {
		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		endTime.QuadPart -= m_startTime.QuadPart; 
		endTime.QuadPart *= 1000000; 
		endTime.QuadPart /= frequency.QuadPart;

		if(endTime.HighPart != 0) {
			return -1;
		} else {
			return endTime.LowPart;
		}
	}

	std::string		m_functionName;
	LARGE_INTEGER	m_startTime;
};

#ifdef _PROFILING
#define _PROFILE eMUCore::profiler_t profiler(__FUNCTION__)
#else
#define _PROFILE
#endif

extern const std::string c_loggerMessageHeader[];
extern const std::string c_loggerMessageCSSClass[];
extern const std::string c_loggerMessageIconPath[];
extern const unsigned char c_loggerMessageColor[];

class eMUCORE_DECLSPEC logger_t {
public:
	enum logMessageType_t {
		_MESSAGE_DEBUG = 0,
		_MESSAGE_INFO,
		_MESSAGE_WARNING,
		_MESSAGE_ERROR,
		_MESSAGE_PROTOCOL
	};

	logger_t(const std::string &fileName, size_t maxStoredLogsCount);
	~logger_t();

	void startup() throw(eMUCore::exception_t);
	std::stringstream& in(logMessageType_t logMessageType);
	inline std::stringstream& append() { return m_stream; }
	void out();

private:
	logger_t();
	logger_t(const logger_t&);
	logger_t& operator=(const logger_t&);

	inline void setColor(unsigned short color) { SetConsoleTextAttribute(m_stdOutput, color); }

	std::string				m_fileName;
	std::stringstream		m_date;
	std::stringstream		m_stream;
	synchronizer_t			m_synchronizer;
	xmlDocPtr				m_file;
	xmlNodePtr				m_tableNode;
	size_t					m_maxStoredLogCount;
	size_t					m_storedLogCount;
	logMessageType_t		m_currentMessageType;
	HANDLE					m_stdOutput;
};

const size_t c_ioDataMaxSize = 8192;

struct eMUCORE_DECLSPEC ioBuffer_t {
public:
	enum ioBufferType_t {
		_IO_RECV_BUFFER = 0x500,
		_IO_SEND_BUFFER
	};

	ioBuffer_t();
	virtual ~ioBuffer_t() {}

	inline void clearData() {
		memset(m_data, 0, c_ioDataMaxSize);
		m_dataSize = 0;
	}

	WSAOVERLAPPED	m_wsaOverlapped;
	WSABUF			m_wsaBuff;
	unsigned char	m_data[c_ioDataMaxSize];
	size_t			m_dataSize;
	ioBufferType_t	m_type;

private:
	ioBuffer_t(const ioBuffer_t&);
	ioBuffer_t& operator=(const ioBuffer_t&);
};

struct eMUCORE_DECLSPEC ioSendBuffer_t: public ioBuffer_t {
public:
	ioSendBuffer_t();

	inline void clearSecondData() { 
		memset(m_secondData, 0, c_ioDataMaxSize);
		m_secondDataSize = 0;
	}

	inline void moveSecondData() {
		memcpy(m_data, m_secondData, m_secondDataSize);
		m_dataSize = m_secondDataSize;
		m_wsaBuff.len = m_secondDataSize;
	}

	inline void mergeSecondData(const unsigned char *data, size_t dataSize) {
		memcpy(&m_secondData[m_secondDataSize], data, dataSize);
		m_secondDataSize += dataSize;
	}

	bool			m_dataLocked;
	unsigned char	m_secondData[c_ioDataMaxSize];
	size_t			m_secondDataSize;

private:
	ioSendBuffer_t(const ioSendBuffer_t&);
	ioSendBuffer_t& operator=(const ioSendBuffer_t&);
};

class eMUCORE_DECLSPEC socketContext_t {
public:
	typedef boost::function1<void, socketContext_t&> callback_t;

	socketContext_t(int index);
	virtual ~socketContext_t() {}

	void close();

	friend std::ostream& operator<<(std::ostream &out, const socketContext_t &context) {
		out << "[" << context.m_index << "][" << context.m_ipAddress << "]";
		return out;
	}

	inline void setActive() { m_active = true; }
	inline void setInactive() { m_active = false; }
	inline bool isActive() const { return m_active; }

	inline void setSocket(SOCKET socket) { m_socket = socket; }
	inline SOCKET getSocket() const { return m_socket; }

	inline ioBuffer_t& getRecvBuffer() { return m_recvBuffer; }
	inline ioSendBuffer_t& getSendBuffer() { return m_sendBuffer; }

	inline void setIpAddress(const std::string &ipAddress) { m_ipAddress = ipAddress; }
	inline const std::string& getIpAddress() const { return m_ipAddress; }

	inline void setPort(unsigned short port) { m_port = port; }
	inline unsigned short getPort() const { return m_port; }

	inline int getIndex() const { return m_index; }

	inline void setCallbacks(const socketContext_t::callback_t &onAttach,
							const socketContext_t::callback_t &onReceive,
							const socketContext_t::callback_t &onClose) {
		m_onAttach = onAttach;
		m_onReceive = onReceive;
		m_onClose = onClose;
	}

	inline void onAttach() { m_onAttach(*this); }
	inline void onReceive() { m_onReceive(*this); }
	inline void onClose() { m_onClose(*this); }

	bool operator!() { return !this->isActive(); }
	bool operator==(const int &key) { return key == m_index; }

protected:
	std::string			m_ipAddress;
	ioBuffer_t			m_recvBuffer;
	ioSendBuffer_t		m_sendBuffer;
	bool				m_active;
	SOCKET				m_socket;
	unsigned short		m_port;
	int					m_index;
	callback_t			m_onAttach;
	callback_t			m_onReceive;
	callback_t			m_onClose;

private:
	socketContext_t();
	socketContext_t(const socketContext_t&);
	socketContext_t& operator=(const socketContext_t&);
};

template<typename T>
class socketContextManager_t {
public:
	socketContextManager_t() {}

	socketContextManager_t(size_t size,
						const socketContext_t::callback_t &onAttach,
						const socketContext_t::callback_t &onReceive,
						const socketContext_t::callback_t &onClose) {
		for(size_t i = 0; i < size; ++i) {
			T *ctx = new T(i);
			ctx->setCallbacks(onAttach, onReceive, onClose);

			m_list.push_back(ctx);
		}
	}

	~socketContextManager_t() {
		for(size_t i = 0; i < m_list.size(); ++i) {
			delete m_list[i];
		}

		m_list.clear();
	}

	inline size_t getCount() const { return m_list.size(); }
	T& operator[](size_t index) { return *m_list[index]; }

	template<typename key_t>
	T& find(const key_t& key) {
		for(size_t i = 0; i < m_list.size(); ++i) {
			if((*m_list[i]) == key) {
				return *m_list[i];
			}
		}

		eMUCore::exception_t e;
		e.in() << "Couldn't found object by key [" << key << "]";
		throw e;
	}

	T* findFree() {
		for(size_t i = 0; i < m_list.size(); ++i) {
			if(!(*m_list[i])) {
				return m_list[i];
			}
		}

		return NULL;
	}

private:
	socketContextManager_t(const socketContextManager_t&);
	socketContextManager_t& operator=(const socketContextManager_t&);

	std::vector<T*> m_list;
};

class eMUCORE_DECLSPEC iocpEngine_t {
public:
	iocpEngine_t(logger_t &logger,
					synchronizer_t &synchronizer);

	~iocpEngine_t();

	void startup() throw(eMUCore::exception_t);
	void attach(socketContext_t &context) const;
	void detach(socketContext_t &context) const;
	void write(socketContext_t &context, const unsigned char *data, size_t dataSize) const;

private:
	iocpEngine_t();
	iocpEngine_t(const iocpEngine_t&);
	iocpEngine_t& operator=(const iocpEngine_t&);

	void dequeueReceive(socketContext_t &context) const;
	void dequeueSend(socketContext_t &context) const;
	void dequeueError(socketContext_t &context, int lastError) const;

	static DWORD worker(iocpEngine_t *instance);

	logger_t						&m_logger;
	synchronizer_t					&m_synchronizer;
	PHANDLE							m_workerThread;
	HANDLE							m_ioCompletionPort;
	size_t							m_workerThreadCount;
};

class eMUCORE_DECLSPEC tcpServer_t {
public:
	typedef boost::function0<socketContext_t*> callback_t;

	tcpServer_t(logger_t &logger,
				iocpEngine_t &iocpEngine,
				const callback_t &onAllocate,
				unsigned short listenPort);

	~tcpServer_t();

	void startup() throw (eMUCore::exception_t);

	inline unsigned short getListenPort() const { return m_listenPort; }

private:
	tcpServer_t();
	tcpServer_t(const tcpServer_t&);
	tcpServer_t& operator=(const tcpServer_t&);

	static DWORD worker(tcpServer_t *instance);

	logger_t						&m_logger;
	iocpEngine_t					&m_iocpEngine;
	SOCKET							m_socket;
	HANDLE							m_acceptWorkerThread;
	unsigned short					m_listenPort;
	callback_t						m_onAllocate;
};

class eMUCORE_DECLSPEC tcpClient_t {
public:
	typedef boost::function0<void> onConnect_t;
	typedef boost::function0<void> onClose_t;
	typedef boost::function2<void, unsigned char*, size_t> onReceive_t;

	tcpClient_t(logger_t &logger,
				synchronizer_t &synchronizer,
				const onConnect_t &onConnect,
				const onReceive_t &onReceive,
				const onClose_t &onClose);

	~tcpClient_t();

	bool connect(const std::string &hostname, unsigned short port) throw(eMUCore::exception_t);
	void close();
	void send(const unsigned char *data, size_t dataSize) const;
	void worker();
	inline bool isActive() const { return m_active; }
	inline const std::string& getHostname() const { return m_hostname; }
	inline unsigned short getPort() const { return m_port; }

private:
	tcpClient_t();
	tcpClient_t(const tcpClient_t&);
	tcpClient_t& operator=(const tcpClient_t&);

	std::string		m_hostname;
	logger_t		&m_logger;
	synchronizer_t	&m_synchronizer;
	SOCKET			m_socket;
	unsigned short	m_port;
	bool			m_active;
	onConnect_t		m_onConnect;
	onReceive_t		m_onReceive;
	onClose_t		m_onClose;
};

class eMUCORE_DECLSPEC udpSocket_t {
public:
	typedef boost::function3<void, sockaddr_in&, unsigned char*, size_t> callback_t;

	udpSocket_t(logger_t &logger,
				const callback_t &onReceive,
				unsigned short port);

	~udpSocket_t();

	void startup() throw(eMUCore::exception_t);

	void worker() const;

	void send(std::string hostname,
				unsigned short port,
				const unsigned char *buffer,
				size_t bufferSize) const;

	unsigned short getPort() const { return m_port; }

private:
	udpSocket_t();
	udpSocket_t(const udpSocket_t&);
	udpSocket_t& operator=(const udpSocket_t&);

	logger_t						&m_logger;
	SOCKET							m_socket;
	unsigned short					m_port;
	callback_t						m_onReceive;
};

class eMUCORE_DECLSPEC xmlConfig_t {
public:
	xmlConfig_t();
	~xmlConfig_t();

	void open(const std::string &fileName, const std::string &rootNodeName) throw(eMUCore::exception_t);

	template <typename T>
	T readFromNode(const std::string &nodeName, const T &defaultValue) {
		m_node = m_rootNode->children;

		if(m_node != NULL) {
			while(m_node) {
				if(xmlStrcmp(m_node->name, reinterpret_cast<const xmlChar*>(nodeName.c_str())) == 0) {
					xmlChar *readValue = xmlNodeListGetString(m_configFile, m_node->children, 1);

					if(readValue != NULL) {
						try {
							return boost::lexical_cast<T>(std::string(reinterpret_cast<char*>(readValue)));
						} catch(std::exception &lc) {
							eMUCore::exception_t e;
							e.in() << "[xmlConfig_t::readFromNode()] " << lc.what() << " :: node [" << nodeName 
									<< "] value [" << readValue << "].";
							throw e;
						}
					} else {
						break;
					}
				} else {
					m_node = m_node->next;
					continue;
				}
			}
		}

		return defaultValue;
	}

	template <typename T>
	T readFromProperty(const std::string &nodeName, const std::string &propertyName, const T &defaultValue) const {
		if(xmlStrcmp(m_node->name, reinterpret_cast<const xmlChar*>(nodeName.c_str())) == 0) {
			xmlChar *readValue = xmlGetProp(m_node, reinterpret_cast<const xmlChar*>(propertyName.c_str()));

			if(readValue != NULL) {
				try {
					return boost::lexical_cast<T>(std::string(reinterpret_cast<char*>(readValue)));
				} catch(std::exception &lc) {
					eMUCore::exception_t e;
					e.in() << "[xmlConfig_t::readFromProperty()] " << lc.what() << " :: property [" << propertyName 
							<< "] value [" << readValue << "].";
					throw e;
				}
			}
		}

		return defaultValue;
	}

	bool nextNode();
	
private:
	xmlConfig_t(const xmlConfig_t&);
	xmlConfig_t& operator=(const xmlConfig_t&);

	xmlDocPtr	m_configFile;
	xmlNodePtr	m_rootNode;
	xmlNodePtr	m_node;
	bool		m_firstIteration;
};

class eMUCORE_DECLSPEC packet_t {
public:
	packet_t();
	packet_t(const unsigned char *data);

	friend std::ostream& operator<<(std::ostream &out, const packet_t &packet) {
		out << "[";

		for(size_t i = 0; i < packet.m_size; ++i) {
			out << std::setfill('0')
				<< std::setw(2)
				<< std::hex
				<< std::uppercase
				<< static_cast<int>(packet.m_data[i]);

			if(i < packet.m_size - 1) {
				out << " ";
			}
		}

		out << "]" << std::dec;
		return out;
	}

	void construct(unsigned char headerId, unsigned char protocolId);
	void setCryptSerial(unsigned char cryptSerial);
	void clear();
	
	template<typename T> 
	void insert(size_t offset, const T &value) {
		size_t valueSize = sizeof(T);

		if(offset + valueSize <= m_maxSize) {
			memcpy(&m_data[offset], &value, valueSize);
			this->increaseSize(valueSize);
		} else {
			exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::insert()] value = " << value 
				<< ", offset = " << offset << " is out of range.";
			throw e;
		}
	}

	void insertString(size_t offset, const std::string &value, size_t totalSize);

	template<typename T>
	T read(size_t offset) const {
		size_t valueSize = sizeof(T);

		if(offset + valueSize <= m_size) {
			T value;
			memcpy(&value, &m_data[offset], sizeof(T));
			return value;
		} else {
			exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::read()] valueSize = " << valueSize
				<< ", offset = " << offset << ", size = " << m_size << " is out of range.";
			throw e;
		}
	}

	std::string readString(size_t offset, size_t size) const;

	inline unsigned char getHeaderId() const { return m_headerId; }
	inline size_t getSize() const { return m_size; }
	inline unsigned char getProtocolId() const { return m_protocolId; }
	inline const unsigned char* getData() const { return m_data; }
	inline size_t getHeaderSize() const { return m_headerSize; }
	inline bool isCrypted() const { return isRawDataCrypted(m_data); }

	static size_t getRawDataSize(const unsigned char *rawData);
	static size_t getCryptedDataPointer(const unsigned char *rawData);
	static bool isRawDataCrypted(const unsigned char *rawData);

private:
	void increaseSize(size_t elementSize);

	unsigned char	m_headerId;
	size_t			m_size;
	unsigned char	m_protocolId;
	unsigned char	m_data[c_ioDataMaxSize];
	size_t			m_headerSize;
	size_t			m_maxSize;
};

class eMUCORE_DECLSPEC scheduler_t {
public:
	enum scheduleType_t {
		_SCHEDULE_SYNCHRONIZED = 0x37A,
		_SCHEDULE_NONSYNCHRONIZED
	};

	scheduler_t(synchronizer_t &synchronizer):
	  m_synchronizer(synchronizer) {}

	void insert(scheduleType_t type, const boost::function0<void> &callback, time_t delay);
	void worker();

private:
	scheduler_t();
	scheduler_t(const scheduler_t&);
	scheduler_t& operator=(const scheduler_t&);

	struct schedule_t {
		boost::function0<void>	m_callback;
		scheduleType_t			m_type;
		time_t					m_delay;
		time_t					m_lastExecuteTime;
	};

	std::vector<schedule_t> m_list;
	synchronizer_t			&m_synchronizer;
};
};

#pragma warning(default: 4251)
//#pragma warning(default: 4290)

#endif // eMU_CORE_CORE_H