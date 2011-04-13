#ifndef eMU_GAMESERVER_MONSTER_H
#define eMU_GAMESERVER_MONSTER_H

#include <vector>
#include <map>
#include <string>
#include <boost/function.hpp>
#include "gameobject.h"

class monsterAttributesManager_t {
public:
	struct monsterAttributes_t {
		std::string		m_name;
		unsigned short	m_level;
		unsigned int	m_maxHealth;
		unsigned int	m_maxMana;
		unsigned int	m_magicDefense;
		unsigned int	m_attackSuccessRate;
		unsigned int	m_defenseSuccessRate;
		unsigned int	m_attackSpeed;
		unsigned int	m_attackType;
		unsigned short	m_attackRange;
		unsigned short  m_viewRange;
		unsigned short	m_moveRange;
		unsigned int	m_moveSpeed;
		unsigned int	m_respawnTime;
		unsigned int	m_itemDropRate;
		unsigned int	m_maxItemDropLevel;
		unsigned int	m_moneyDropRate;	
		bool			m_windProtect;
		bool			m_poisonProtect;
		bool			m_iceProtect;
		bool			m_electricProtect;
		bool			m_fireProtect;
		unsigned int	m_minDamage;
		unsigned int	m_maxDamage;
		unsigned int	m_defense;
	};

	void startup(const std::string &fileName);
	const monsterAttributes_t& operator[](int monsterId);

private:
	std::map<int, monsterAttributes_t> m_attributesList;
};

class monster_t: public gameObject_t {
public:
	monster_t(int index, const monsterAttributesManager_t::monsterAttributes_t &attr);

	inline int index() { return m_index; }

	inline void id(int id) { m_id = id; }
	inline int id() const { return m_id; }

	const monsterAttributesManager_t::monsterAttributes_t& attributes() const { return m_attributes; }

	const std::string& name() const { return m_attributes.m_name; }

	unsigned short viewRange() const { return m_attributes.m_viewRange; }

	unsigned int health() const { return m_health; }
	void health(unsigned short health) { m_health = health; }

	unsigned int mana() const { return m_mana; }
	void mana(unsigned short mana) { m_mana = mana; }

	unsigned char mapId() const { return m_mapId; }
	void mapId(unsigned char mapId) { m_mapId = mapId; }

	const eMUShared::position_t& position() const { return m_position; }
	void position(eMUShared::position_t &pos) { m_position = pos; }

	unsigned char direction() const { return m_direction; }
	void direction(unsigned char direction) { m_direction = direction; }

private:
	int	m_index;
	int	m_id;
	eMUShared::position_t m_position;
	unsigned char m_mapId;
	unsigned char m_direction;
	unsigned int m_health;
	unsigned int m_mana;
	const monsterAttributesManager_t::monsterAttributes_t &m_attributes;
};

class monsterManager_t {
public:
	monsterManager_t(monsterAttributesManager_t	&monsterAttributesManager,
						int startIndex);

	void startup(const std::string &monstersFileName,
					const boost::function1<void, monster_t*> &registerObjectCallback);
	void cleanup();
	size_t count() const { return m_monsterList.size(); }

	monster_t& operator[](int index) { return *m_monsterList[index - m_startIndex]; }

private:
	monsterManager_t();
	monsterManager_t(const monsterManager_t&);
	monsterManager_t& operator=(const monsterManager_t&);

	monsterAttributesManager_t		&m_monsterAttributesManager;
	std::vector<monster_t*>			m_monsterList;
	int								m_startIndex;
};

#endif // eMU_GAMESERVER_MONSTER_H