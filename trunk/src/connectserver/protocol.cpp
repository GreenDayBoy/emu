#include "protocol.h"
#include "enum.h"

protocol_t::protocol_t(protocolExecutorInterface_t &iface,
						const sendCallback_t &sendCallback):
  m_executorInterface(iface),
  m_sendCallback(sendCallback) {}

void protocol_t::core(connectServerUser_t &user,
						const eMUCore::packet_t &packet) const {
	switch(packet.getProtocolId()) {
	case protocol_e::_serverListManage:
		switch(packet.getData()[3]) {
		case protocol_e::serverListManage_e::_serverSelect:
			this->parseServerSelectRequest(user, packet);
			break;

		case protocol_e::serverListManage_e::_get:
			this->parseServerListRequest(user, packet);
			break;
		}
		break;
	}
}

void protocol_t::sendHandshake(connectServerUser_t &user) const {
	eMUCore::packet_t packet;
	packet.construct(0xC1, protocol_e::_handshake);
	packet.insert<unsigned char>(3, 0x01);

	m_sendCallback(user, packet);
}

void protocol_t::parseServerListRequest(connectServerUser_t &user, 
											const eMUCore::packet_t &packet) const {
	unsigned char check = packet.read<unsigned char>(3);

	if(check != 0) {
		m_executorInterface.onServerListRequest(user);
	}
}

void protocol_t::sendServerListAnswer(connectServerUser_t &user,
										const serverList_t::serverAttributesList_t &list) const {
	eMUCore::packet_t packet;
	packet.construct(0xC2, protocol_e::_serverListManage);
	packet.insert<unsigned char>(4, protocol_e::serverListManage_e::_get);

	size_t serversCount = 0;

	for(serverList_t::serverAttributesList_t::const_iterator i = list.begin(); i != list.end(); ++i) {
		const serverList_t::serverAttributes_t &attr = i->second;

		unsigned short step = serversCount * 4;

		if(attr.m_active) {
			packet.insert<unsigned short>(7 + step, attr.m_code);
			packet.insert<unsigned char>(9 + step, attr.m_load);
			packet.insert<unsigned char>(10 + step, 0x00);
			++serversCount;
		}
	}

	// WTF: Servers count as big endian?
	packet.insert<unsigned short>(5, _byteswap_ushort(serversCount));

	m_sendCallback(user, packet);
}

void protocol_t::parseServerSelectRequest(connectServerUser_t &user, 
											const eMUCore::packet_t &packet) const {
	unsigned short serverCode = packet.read<unsigned short>(4);
	m_executorInterface.onServerSelectRequest(user, serverCode);
}

void protocol_t::sendServerSelectAnswer(connectServerUser_t &user, 
										const serverList_t::serverAttributes_t &attr) const {
	eMUCore::packet_t packet;

	packet.construct(0xC1, protocol_e::_serverListManage);
	packet.insert<unsigned char>(3, protocol_e::serverListManage_e::_serverSelect);

	if(!eMUCore::isIpAddress(attr.m_address)) {
		std::string ipAddress = eMUCore::convertToIpAddress(attr.m_address);
		packet.insertString(4, ipAddress, 16);
	} else {
		packet.insertString(4, attr.m_address, 16);
	}

	packet.insert<unsigned short>(20, attr.m_port);

	m_sendCallback(user, packet);
}