#include "core.h"

using namespace eMUCore;

packet_t::packet_t():
  m_headerId(0),
  m_size(0),
  m_protocolId(0),
  m_headerSize(0) {
	memset(m_data, 0, c_ioDataMaxSize);
}

packet_t::packet_t(const unsigned char *data):
  m_headerId(0),
  m_size(0),
  m_protocolId(0),
  m_headerSize(0) {
	if(data[0] == 0xC1 || data[0] == 0xC3) {
		m_headerSize = 3;

		m_headerId = data[0];
		m_size = data[1];
		m_protocolId = data[2];
		m_maxSize = 255;
	} else if(data[0] == 0xC2 || data[0] == 0xC4) {
		m_headerSize = 4;

		m_headerId = data[0];
		m_size = MAKEWORD(data[2], data[1]);
		m_protocolId = data[3];
		m_maxSize = c_ioDataMaxSize;
	} else {
		exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::packet_t()] invalid headerId [0x" 
				<< std::hex << std::uppercase << static_cast<unsigned int>(data[0]) << "].";
		throw e;
	}

	if(m_size <= c_ioDataMaxSize) {
		memset(m_data, 0, c_ioDataMaxSize);
		memcpy(m_data, data, m_size);
	} else {
		exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::packet_t()] packet size [" << m_size << "] is too big.";
		throw e;
	}
}

void packet_t::construct(unsigned char headerId, unsigned char protocolId) {
	if(headerId == 0xC1 || headerId == 0xC3) {
		m_headerSize = 3;

		m_headerId = m_data[0] = headerId;
		m_protocolId = m_data[2] = protocolId;
		m_maxSize = 255;
	} else if(headerId == 0xC2 || headerId == 0xC4) {
		m_headerSize = 4;

		m_headerId = m_data[0] = headerId;
		m_protocolId = m_data[3] = protocolId;
		m_maxSize = c_ioDataMaxSize;
	} else {
		exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::construct()] Invalid headerId [0x" 
				<< std::hex << std::uppercase << static_cast<unsigned int>(headerId) << "].";
		throw e;
	}

	this->increaseSize(m_headerSize);
}

void packet_t::insertString(size_t offset, const std::string &value, size_t totalSize) {
	if(offset + totalSize <= m_maxSize) {
		size_t stringSize = std::min(value.size(), totalSize);

		for(size_t i = 0; i < stringSize; ++i) {
			this->insert<char>(offset + i, value[i]);
		}

		// Fill string with 0x00.
		// By std::min stringSize never be greater than totalSize.
		size_t fillSize = totalSize - stringSize;
		for(size_t i = 0; i < fillSize; ++i) {
			this->insert<char>(offset + stringSize + i, 0x00);
		}
	} else {
		exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::insertString()] value [" << value 
				<< "] offset [" << offset << "] is out of range.";
		throw e;
	}
}

std::string packet_t::readString(size_t offset, size_t size) const {
	if(offset + size <= m_maxSize) {
		std::string value;

		for(size_t i = 0; i < size; ++i) {
			if(m_data[offset + i] == 0) {
				break;
			} else {
				value.push_back(m_data[offset + i]);
			}
		}

		return value;
	} else {
		exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::readString()] size [" << size
				<< "] offset [" << offset << "] is out of range.";
		throw e;
	}
}

void packet_t::increaseSize(size_t elementSize) {
	if(m_headerId == 0xC1 || m_headerId == 0xC3) {
		m_size = m_data[1] = m_data[1] + elementSize;
	} else if(m_headerId == 0xC2 || m_headerId == 0xC4) {
		m_size = MAKEWORD(m_data[2], m_data[1]) + elementSize;
		m_data[1] = HIBYTE(m_size);
		m_data[2] = LOBYTE(m_size);
	}
}

size_t packet_t::rawDataSize(const unsigned char *rawData) {
	if(rawData[0] == 0xC1 || rawData[0] == 0xC3) {
		return rawData[1];
	} else if(rawData[0] == 0xC2 || rawData[0] == 0xC4) {
		return MAKEWORD(rawData[2], rawData[1]);
	} else {
		exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << " [packet_t::getRawDataSize()] Invalid headerId [0x" 
				<< std::hex << std::uppercase << static_cast<unsigned int>(rawData[0]) << "].";
		throw e;
	}
}

bool packet_t::rawDataCrypted(const unsigned char *rawData) {
	if(rawData[0] == 0xC3 || rawData[0] == 0xC4) {
		return true;
	} else {
		return false;
	}
}

size_t packet_t::cryptedDataPointer(const unsigned char *rawData) {
	if(rawData[0] == 0xC3) {
		return 2;
	} else if(rawData[0] == 0xC4) {
		return 3;
	}

	return 0;
}

void packet_t::cryptSerial(unsigned char cryptSerial) {
	if(m_headerId == 0xC3) {
		m_data[1] = cryptSerial;
	} else if(m_headerId == 0xC4) {
		m_data[2] = cryptSerial;
	}
}

void packet_t::clear() {
	m_headerId = 0;
	m_size = 0;
	m_protocolId = 0;
	m_headerSize = 0;
	memset(m_data, 0, c_ioDataMaxSize);
}