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
		unsigned int	m_windProtect;
		unsigned int	m_poisonProtect;
		unsigned int	m_iceProtect;
		unsigned int	m_electricProtect;
		unsigned int	m_fireProtect;
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
	monster_t(int index);

	void setAttributes(const monsterAttributesManager_t::monsterAttributes_t &attr);

	inline int getIndex() { return m_index; }

	inline void setId(int id) { m_id = id; }
	inline int getId() const { return m_id; }

private:
	int				m_index;
	int				m_id;
	unsigned int	m_magicDefense;
	unsigned int	m_attackSuccessRate;
	unsigned int	m_defenseSuccessRate;
	unsigned int	m_attackSpeed;
	unsigned int	m_attackType;
	unsigned short	m_attackRange;
	unsigned short	m_moveRange;
	unsigned int	m_moveSpeed;
	unsigned int	m_respawnTime;
	unsigned int	m_itemDropRate;
	unsigned int	m_maxItemDropLevel;
	unsigned int	m_moneyDropRate;	
	unsigned int	m_windProtect;
	unsigned int	m_poisonProtect;
	unsigned int	m_iceProtect;
	unsigned int	m_electricProtect;
	unsigned int	m_fireProtect;
	unsigned int	m_minDamage;
	unsigned int	m_maxDamage;
	unsigned int	m_defense;
	bool			m_isNpc;
};

class monsterManager_t {
public:
	monsterManager_t(monsterAttributesManager_t	&monsterAttributesManager,
						int startIndex);

	void startup(const std::string &monstersFileName,
					const boost::function1<void, monster_t*> &registerObjectCallback);
	void cleanup();
	size_t getCount() const { return m_monsterList.size(); }

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