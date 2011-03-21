#ifndef eMU_SHARED_H
#define eMU_SHARED_H

#include <string>
#include <vector>

namespace eMUShared {
struct characterListAttributes_t {
	std::string m_name;
	unsigned char m_race;
	unsigned short m_level;
	unsigned char m_controlCode;
};

typedef std::vector<characterListAttributes_t> characterList_t;

struct characterAttributes_t {
	std::string		m_name;
	unsigned char	m_race;
	unsigned char	m_posX;
	unsigned char	m_posY;
	unsigned char	m_mapId;
	unsigned char	m_direction;
	unsigned int	m_experience;
	unsigned short	m_level;
	unsigned short	m_levelUpPoints;
	unsigned short	m_strength;
	unsigned short	m_agility;
	unsigned short	m_vitality;
	unsigned short	m_energy;
	unsigned short	m_health;
	unsigned short	m_maxHealth;
	unsigned short	m_mana;
	unsigned short	m_maxMana;
	unsigned short	m_shield;
	unsigned short	m_maxShield;
	unsigned short	m_stamina;
	unsigned short	m_maxStamina;
	unsigned int	m_money;
	unsigned char	m_pkLevel;
	unsigned char	m_controlCode;
	unsigned short	m_addPoints;
	unsigned short	m_maxAddPoints;
	unsigned short	m_command;
	unsigned short	m_minusPoints;
	unsigned short	m_maxMinusPoints;
};
};

#endif // eMU_SHARED_H