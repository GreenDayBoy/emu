#include "gate.h"
#include "..\core\core.h"

gate_t::gate_t(int id,
				int type,
				unsigned char mapId,
				unsigned char x1,
				unsigned char y1,
				unsigned char x2,
				unsigned char y2,
				int destId,
				unsigned char direction,
				unsigned short requiredLevel):
  m_id(id),
  m_type(type),
  m_mapId(mapId),
  m_x1(x1),
  m_y1(y1),
  m_x2(x2),
  m_y2(y2),
  m_destId(destId),
  m_direction(direction),
  m_requiredLevel(requiredLevel) {}

bool gate_t::isInGate(unsigned char x, unsigned char y) const {
	if(x >= m_x1 && x <= m_x2
		&& y >= m_y1 && y <= m_y2) {
		return true;
	} else {
		return false;
	}
}

map_t::position_t gate_t::getRandomPosition() const {
	unsigned char x = eMUCore::role<unsigned char>(m_x1, m_x2);
	unsigned char y = eMUCore::role<unsigned char>(m_y1, m_y2);

	return map_t::position_t(x, y);
}

gateManager_t::~gateManager_t() {
	for(gateList_t::iterator i = m_gateList.begin(); i != m_gateList.end(); ++i) {
		delete i->second;
	}

	m_gateList.clear();
}

void gateManager_t::startup(const std::string &filename) {
	eMUCore::xmlConfig_t gateFile;
	gateFile.open(filename, "gates");

	while(gateFile.nextNode()) {
		gate_t *gate = new gate_t(gateFile.readFromProperty<int>("gate", "id", -1),
									gateFile.readFromProperty<int>("gate", "type", -1),
									gateFile.readFromProperty<int>("gate", "mapId", 0),
									gateFile.readFromProperty<int>("gate", "x1", 0),
									gateFile.readFromProperty<int>("gate", "y1", 0),
									gateFile.readFromProperty<int>("gate", "x2", 0),
									gateFile.readFromProperty<int>("gate", "y2", 0),
									gateFile.readFromProperty<int>("gate", "destId", -1),
									gateFile.readFromProperty<int>("gate", "dir", 0),
									gateFile.readFromProperty<unsigned short>("gate", "level", 0));
		m_gateList[gate->getId()] = gate;
	}
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