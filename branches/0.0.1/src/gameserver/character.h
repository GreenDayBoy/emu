#ifndef eMU_GAMESERVER_CHARACTER_H
#define eMU_GAMESERVER_CHARACTER_H

#include "..\shared\shared.h"
#include "gameobject.h"

class gameServerUser_t; // definition in user.h.

class character_t: public gameObject_t {
public:
	character_t(gameServerUser_t &owner);

	void reset();

	inline gameServerUser_t& getOwner() { return m_owner; }

	inline bool isActive() { return m_active; }
	inline void setActive() { m_active = true; }
	inline void setInactive() { m_active = false; }

	void setAttributes(const eMUShared::characterAttributes_t &attr);

	inline unsigned int getLastMoveTime() const { return m_lastMoveTime; }
	inline void setLastMoveTime(unsigned int time) { m_lastMoveTime = time; }

	inline unsigned char getRace() const { return m_race; }
	inline void setRace(unsigned char race) { m_race = race; }

	inline unsigned int getExperience() const { return m_experience; }
	inline void setExperience(unsigned int experience) { m_experience = experience; }

	inline unsigned int getNextExperience() const { return m_nextExperience; }
	inline void setNextExperience(unsigned int nextExperience) { m_nextExperience = nextExperience; }

	inline unsigned short getLevelUpPoints() const { return m_levelUpPoints; }
	inline void setLevelUpPoints(unsigned short levelUpPoints) { m_levelUpPoints = levelUpPoints; }

	inline unsigned short getStrength() const { return m_strength; }
	inline void setStrength(unsigned short strength) { m_strength = strength; }

	inline unsigned short getAgility() const { return m_agility; }
	inline void setAgility(unsigned short agility) { m_agility = agility; }

	inline unsigned short getVitality() const { return m_vitality; }
	inline void setVitality(unsigned short vitality) { m_vitality = vitality; }

	inline unsigned short getEnergy() const { return m_energy; }
	inline void setEnergy(unsigned short energy) { m_energy = energy; }

	inline unsigned short getCommand() const { return m_command; }
	inline void setCommand(unsigned short command) { m_command = command; }

	inline unsigned short getShield() const { return m_shield; }
	inline void setShield(unsigned short shield) { m_shield = shield; }

	inline unsigned short getMaxShield() const { return m_maxShield; }
	inline void setMaxShield(unsigned short maxShield) { m_maxShield = maxShield; }

	inline unsigned short getStamina() const { return m_stamina; }
	inline void setStamina(unsigned short stamina) { m_stamina = stamina; }

	inline unsigned short getMaxStamina() const { return m_maxStamina; }
	inline void setMaxStamina(unsigned short maxStamina) { m_maxStamina = maxStamina; }

	inline unsigned int getMoney() const { return m_money; }
	inline void setMoney(unsigned int money) { m_money = money; }

	inline unsigned char getPkLevel() const { return m_pkLevel; }
	inline void setPkLevel(unsigned char pkLevel) { m_pkLevel = pkLevel; }
	
	inline unsigned short getAddPoints() const { return m_addPoints; }
	inline void setAddPoints(unsigned short addPoints) { m_addPoints = addPoints; }

	inline unsigned short getMaxAddPoints() const { return m_maxAddPoints; }
	inline void setMaxAddPoints(unsigned short maxAddPoints) { m_maxAddPoints = maxAddPoints; }

	inline unsigned short getMinusPoints() const { return m_minusPoints; }
	inline void setMinusPoints(unsigned short minusPoints) { m_minusPoints = minusPoints; }

	inline unsigned short getMaxMinusPoints() const { return m_maxMinusPoints; }
	inline void setMaxMinusPoins(unsigned short maxMinusPoints) { m_maxMinusPoints = maxMinusPoints; }

	inline unsigned char getControlCode() const { return m_controlCode; }

	inline const unsigned char* getPreview() const { return m_preview; }
	void setPreview();

	void setPose(unsigned char actionId);
	inline unsigned char getPose() { return m_pose; }

	bool isTeleportEffect() { return m_teleportEffect; }
	void activateTeleportEffect() { m_teleportEffect = true; }
	void deactivateTeleportEffect() { m_teleportEffect = false; }

private:
	character_t();
	character_t(const character_t&);
	character_t& operator=(const character_t&);

	gameServerUser_t	&m_owner;
	bool				m_active;
	unsigned char		m_race;
	unsigned int		m_lastMoveTime;
	unsigned int		m_experience;
	unsigned int		m_nextExperience;
	unsigned short		m_levelUpPoints;
	unsigned short		m_strength;
	unsigned short		m_agility;
	unsigned short		m_vitality;
	unsigned short		m_energy;
	unsigned short		m_shield;
	unsigned short		m_maxShield;
	unsigned short		m_stamina;
	unsigned short		m_maxStamina;
	unsigned int		m_money;
	unsigned char		m_pkLevel;
	unsigned char		m_controlCode;
	unsigned short		m_addPoints;
	unsigned short		m_maxAddPoints;
	unsigned short		m_command;
	unsigned short		m_minusPoints;
	unsigned short		m_maxMinusPoints;
	unsigned char		m_preview[18];
	unsigned char		m_pose;
	bool				m_teleportEffect;
};

#endif // eMU_GAMESERVER_CHARACTER_H