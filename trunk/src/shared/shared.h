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

	void clear() {
		m_name = "";
		m_race = 0;
		m_posX = 0;
		m_posY = 0;
		m_mapId = 0;
		m_direction = 0;
		m_experience = 0;
		m_level = 0;
		m_levelUpPoints = 0;
		m_strength = 0;
		m_agility = 0;
		m_vitality = 0;
		m_energy = 0;
		m_health = 0;
		m_maxHealth = 0;
		m_mana = 0;
		m_maxMana = 0;
		m_shield = 0;
		m_maxShield = 0;
		m_stamina = 0;
		m_maxStamina = 0;
		m_money = 0;
		m_pkLevel = 0;
		m_controlCode = 0;
		m_addPoints = 0;
		m_maxAddPoints = 0;
		m_command = 0;
		m_minusPoints = 0;
		m_maxMinusPoints = 0;
	}
};
};

#endif // eMU_SHARED_H