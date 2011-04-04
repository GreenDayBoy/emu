#include "gate.h"
#include "..\core\core.h"

gate_t::gate_t(int id,
				int type,
				unsigned char mapId,
				const eMUShared::position_t &startPos,
				const eMUShared::position_t &endPos,
				int destId,
				unsigned char direction,
				unsigned short requiredLevel):
  m_id(id),
  m_type(type),
  m_mapId(mapId),
  m_startPos(startPos),
  m_endPos(endPos),
  m_destId(destId),
  m_direction(direction),
  m_requiredLevel(requiredLevel) {}

bool gate_t::isInGate(const eMUShared::position_t &pos) const {
	if(pos.m_x >= m_startPos.m_x && pos.m_x <= m_endPos.m_x
		&& pos.m_y >= m_startPos.m_y && pos.m_y <= m_endPos.m_y) {
		return true;
	} else {
		return false;
	}
}

void gateManager_t::startup(const std::string &filename) {
	eMUCore::xmlConfig_t gateFile;
	gateFile.open(filename, "gates");

	while(gateFile.nextNode()) {
		gate_t *gate = new gate_t(gateFile.readFromProperty<int>("gate", "id", -1),
									gateFile.readFromProperty<int>("gate", "type", -1),
									gateFile.readFromProperty<int>("gate", "mapId", 0),
									eMUShared::position_t(gateFile.readFromProperty<int>("gate", "x1", 0),
															gateFile.readFromProperty<int>("gate", "y1", 0)),
									eMUShared::position_t(gateFile.readFromProperty<int>("gate", "x2", 0),
															gateFile.readFromProperty<int>("gate", "y2", 0)),
									gateFile.readFromProperty<int>("gate", "destId", -1),
									gateFile.readFromProperty<int>("gate", "dir", 0),
									gateFile.readFromProperty<unsigned short>("gate", "level", 0));
		m_gateList[gate->getId()] = gate;
	}

	gateFile.close();
}

void gateManager_t::cleanup() {
	for(gateList_t::iterator i = m_gateList.begin(); i != m_gateList.end(); ++i) {
		delete i->second;
	}

	m_gateList.clear();
}

gate_t& gateManager_t::operator[](int gateId) {
	if(this->isGateExists(gateId)) {
		return *m_gateList[gateId];
	} else {
		eMUCore::exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << "[gateManager_t::operator[]()] No gate [" << gateId << "] found on gate list.";
		throw e;
	}
}

bool gateManager_t::isGateExists(int gateId) const {
	gateList_t::const_iterator iter = m_gateList.find(gateId);

	if(iter != m_gateList.end()) {
		return true;
	} else {
		return false;
	}
}