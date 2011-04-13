#ifndef eMU_GAMESERVER_GATE_H
#define eMU_GAMESERVER_GATE_H

#include <map>
#include "map.h"

class gate_t {
public:
	gate_t() {}
	gate_t(int id,
			int type,
			unsigned char mapId,
			const eMUShared::position_t &startPos,
			const eMUShared::position_t &endPos,
			int destId,
			unsigned char direction,
			unsigned short requiredLevel);

	friend std::ostream& operator<<(std::ostream &out, const gate_t &gate) {
		out << "[" << gate.m_id << "][" << static_cast<int>(gate.m_mapId) 
			<< "]" << gate.m_startPos << "-" << gate.m_endPos
			<< "[" << gate.m_destId << "]";

		return out;
	}

	inline int id() const { return m_id; }
	inline int type() const { return m_type; }
	inline unsigned char mapId() const { return m_mapId; }
	inline const eMUShared::position_t& startPosition() const { return m_startPos; }
	inline const eMUShared::position_t& endPosition() const { return m_endPos; }
	inline int destId() const { return m_destId; }
	inline unsigned char direction() const { return m_direction; }
	inline unsigned short requiredLevel() const { return m_requiredLevel; }

	bool inGate(const eMUShared::position_t &pos) const;

private:
	gate_t(const gate_t&);
	gate_t& operator=(const gate_t&);

	int				m_id;
	int				m_type;
	unsigned char	m_mapId;
	eMUShared::position_t m_startPos;
	eMUShared::position_t m_endPos;
	int				m_destId;
	unsigned char	m_direction;
	unsigned short	m_requiredLevel;
};

class gateManager_t {
public:
	typedef std::map<int, gate_t*> gateList_t;

	gateManager_t() {}

	void startup(const std::string &filename);
	void cleanup();
	gate_t& operator[](int gateId);
	bool gateExists(int gateId) const;

private:
	gateManager_t(const gateManager_t&);
	gateManager_t& operator=(const gateManager_t&);

	gateList_t m_gateList;
};

#endif // eMU_GAMESERVER_GATE_H