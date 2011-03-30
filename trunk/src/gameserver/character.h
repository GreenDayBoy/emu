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
	void setAttributes(const eMUShared::characterAttributes_t &attr);

	inline bool isActive() { return m_active; }
	inline void setActive() { m_active = true; }
	inline void setInactive() { m_active = false; }

	inline unsigned int getLastMoveTime() const { return m_lastMoveTime; }
	inline void setLastMoveTime(unsigned int time) { m_lastMoveTime = time; }

	inline unsigned char getRace() const { return m_attributes.m_race; }
	inline void setRace(unsigned char race) { m_attributes.m_race = race; }

	inline unsigned int getExperience() const { return m_attributes.m_experience; }
	inline void setExperience(unsigned int experience) { m_attributes.m_experience = experience; }

	inline unsigned int getNextExperience() const { return m_nextExperience; }
	inline void setNextExperience(unsigned int nextExperience) { m_nextExperience = nextExperience; }

	inline unsigned short getLevelUpPoints() const { return m_attributes.m_levelUpPoints; }
	inline void setLevelUpPoints(unsigned short levelUpPoints) { m_attributes.m_levelUpPoints = levelUpPoints; }

	inline unsigned short getStrength() const { return m_attributes.m_strength; }
	inline void setStrength(unsigned short strength) { m_attributes.m_strength = strength; }

	inline unsigned short getAgility() const { return m_attributes.m_agility; }
	inline void setAgility(unsigned short agility) { m_attributes.m_agility = agility; }

	inline unsigned short getVitality() const { return m_attributes.m_vitality; }
	inline void setVitality(unsigned short vitality) { m_attributes.m_vitality = vitality; }

	inline unsigned short getEnergy() const { return m_attributes.m_energy; }
	inline void setEnergy(unsigned short energy) { m_attributes.m_energy = energy; }

	inline unsigned short getCommand() const { return m_attributes.m_command; }
	inline void setCommand(unsigned short command) { m_attributes.m_command = command; }

	inline unsigned short getShield() const { return m_attributes.m_shield; }
	inline void setShield(unsigned short shield) { m_attributes.m_shield = shield; }

	inline unsigned short getMaxShield() const { return m_attributes.m_maxShield; }
	inline void setMaxShield(unsigned short maxShield) { m_attributes.m_maxShield = maxShield; }

	inline unsigned short getStamina() const { return m_attributes.m_stamina; }
	inline void setStamina(unsigned short stamina) { m_attributes.m_stamina = stamina; }

	inline unsigned short getMaxStamina() const { return m_attributes.m_maxStamina; }
	inline void setMaxStamina(unsigned short maxStamina) { m_attributes.m_maxStamina = maxStamina; }

	inline unsigned int getMoney() const { return m_attributes.m_money; }
	inline void setMoney(unsigned int money) { m_attributes.m_money = money; }

	inline unsigned char getPkLevel() const { return m_attributes.m_pkLevel; }
	inline void setPkLevel(unsigned char pkLevel) { m_attributes.m_pkLevel = pkLevel; }
	
	inline unsigned short getAddPoints() const { return m_attributes.m_addPoints; }
	inline void setAddPoints(unsigned short addPoints) { m_attributes.m_addPoints = addPoints; }

	inline unsigned short getMaxAddPoints() const { return m_attributes.m_maxAddPoints; }
	inline void setMaxAddPoints(unsigned short maxAddPoints) { m_attributes.m_maxAddPoints = maxAddPoints; }

	inline unsigned short getMinusPoints() const { return m_attributes.m_minusPoints; }
	inline void setMinusPoints(unsigned short minusPoints) { m_attributes.m_minusPoints = minusPoints; }

	unsigned short getMaxMinusPoints() const { return m_attributes.m_maxMinusPoints; }
	void setMaxMinusPoins(unsigned short maxMinusPoints) { m_attributes.m_maxMinusPoints = maxMinusPoints; }

	inline unsigned char getControlCode() const { return m_attributes.m_controlCode; }

	inline const unsigned char* getPreview() const { return m_preview; }
	void setPreview();

	void setPose(unsigned char actionId);
	inline unsigned char getPose() { return m_pose; }

	bool hasTeleportEffect() { return m_hasTeleportEffect; }
	void activateTeleportEffect() { m_hasTeleportEffect = true; }
	void deactivateTeleportEffect() { m_hasTeleportEffect = false; }

	const std::string& getName() const { return m_attributes.m_name; }

	unsigned short getLevel() const { return m_attributes.m_level; }
	void setLevel(unsigned short level) { m_attributes.m_level = level; }

	unsigned int getHealth() const { return m_attributes.m_health; }
	void setHealth(unsigned short health) { m_attributes.m_health = health; }

	unsigned int getMaxHealth() const { return m_attributes.m_maxHealth; }
	void setMaxHealth(unsigned short maxHealth) { m_attributes.m_maxHealth = maxHealth; }

	unsigned int getMana() const { return m_attributes.m_mana; }
	void setMana(unsigned short mana) { m_attributes.m_mana = mana; }

	unsigned int getMaxMana() const { return m_attributes.m_maxMana; }
	void setMaxMana(unsigned short maxMana) { m_attributes.m_maxMana = maxMana; }

	unsigned char getMapId() const { return m_attributes.m_mapId; }
	void setMapId(unsigned char mapId) { m_attributes.m_mapId = mapId; }

	unsigned char getPosX() const { return m_attributes.m_posX; }
	void setPosX(unsigned char posX) { m_attributes.m_posX = posX; }

	unsigned char getPosY() const { return m_attributes.m_posY; }
	void setPosY(unsigned char posY) { m_attributes.m_posY = posY; }

	unsigned char getDirection() const { return m_attributes.m_direction; }
	void setDirection(unsigned char direction) { m_attributes.m_direction = direction; }

	unsigned short getViewRange() const { return m_viewRange; }
	void setViewRange(unsigned char viewRange) { m_viewRange = viewRange; }

private:
	character_t();
	character_t(const character_t&);
	character_t& operator=(const character_t&);

	gameServerUser_t		&m_owner;
	bool					m_active;
	unsigned int			m_lastMoveTime;
	unsigned int			m_nextExperience;
	unsigned char			m_preview[18];
	unsigned char			m_pose;
	unsigned char			m_viewRange;
	bool					m_hasTeleportEffect;
	eMUShared::characterAttributes_t m_attributes;
};

#endif // eMU_GAMESERVER_CHARACTER_H