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
			unsigned char x1,
			unsigned char y1,
			unsigned char x2,
			unsigned char y2,
			int destId,
			unsigned char direction,
			unsigned short requiredLevel);

	friend std::ostream& operator<<(std::ostream &out, const gate_t &gate) {
		out << "[" << gate.m_id << "][" << static_cast<int>(gate.m_mapId) 
			<< "][" << static_cast<int>(gate.m_x1) << "-" << static_cast<int>(gate.m_y1)
			<< "][" << static_cast<int>(gate.m_x2) << "-" << static_cast<int>(gate.m_y2) 
			<< "][" << gate.m_destId << "]";

		return out;
	}

	inline int getId() const { return m_id; }
	inline int getType() const { return m_type; }
	inline unsigned char getMapId() const { return m_mapId; }
	inline unsigned char getX1() const { return m_x1; }
	inline unsigned char getY1() const { return m_y1; }
	inline unsigned char getX2() const { return m_x2; }
	inline unsigned char getY2() const { return m_y2; }
	inline int getDestId() const { return m_destId; }
	inline unsigned char getDirection() const { return m_direction; }
	inline unsigned short getRequiredLevel() const { return m_requiredLevel; }

	bool isInGate(unsigned char x, unsigned char y) const;
	map_t::position_t getRandomPosition() const;

private:
	gate_t(const gate_t&);
	gate_t& operator=(const gate_t&);

	int				m_id;
	int				m_type;
	unsigned char	m_mapId;
	unsigned char	m_x1;
	unsigned char	m_y1;
	unsigned char	m_x2;
	unsigned char	m_y2;
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
	bool isGateExists(int gateId) const;

private:
	gateManager_t(const gateManager_t&);
	gateManager_t& operator=(const gateManager_t&);

	gateList_t m_gateList;
};

#endif // eMU_GAMESERVER_GATE_H