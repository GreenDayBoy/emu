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
	eMUShared::characterAttributes_t& getAttributes() { return m_attributes; }

	inline bool isActive() { return m_active; }
	inline void setActive() { m_active = true; }
	inline void setInactive() { m_active = false; }

	inline unsigned int getLastMoveTime() const { return m_lastMoveTime; }
	inline void setLastMoveTime(unsigned int time) { m_lastMoveTime = time; }

	inline unsigned int getNextExperience() const { return m_nextExperience; }
	inline void setNextExperience(unsigned int nextExperience) { m_nextExperience = nextExperience; }

	inline const unsigned char* getPreview() const { return m_preview; }
	void setPreview();

	void setPose(characterAction_e::type_t actionId);
	inline unsigned char getPose() { return m_pose; }

	bool hasTeleportEffect() { return m_hasTeleportEffect; }
	void activateTeleportEffect() { m_hasTeleportEffect = true; }
	void deactivateTeleportEffect() { m_hasTeleportEffect = false; }

	const std::string& getName() const { return m_attributes.m_name; }

	unsigned char getMapId() const { return m_attributes.m_mapId; }
	void setMapId(unsigned char mapId) { m_attributes.m_mapId = mapId; }

	const eMUShared::position_t& getPosition() const { return m_attributes.m_position; }
	void setPosition(const eMUShared::position_t &pos) { m_attributes.m_position = pos; }

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
	characterPose_e::type_t	m_pose;
	unsigned char			m_viewRange;
	bool					m_hasTeleportEffect;
	eMUShared::characterAttributes_t m_attributes;
};

#endif // eMU_GAMESERVER_CHARACTER_H