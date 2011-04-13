#ifndef eMU_GAMESERVER_CHARACTER_H
#define eMU_GAMESERVER_CHARACTER_H

#include "..\shared\shared.h"
#include "gameobject.h"

class gameServerUser_t; // definition in user.h.

class character_t: public gameObject_t {
public:
	character_t(gameServerUser_t &owner);

	void reset();
	inline gameServerUser_t& owner() { return m_owner; }

	void attributes(const eMUShared::characterAttributes_t &attr);
	eMUShared::characterAttributes_t& attributes() { return m_attributes; }

	inline bool active() { return m_active; }
	inline void activate() { m_active = true; }
	inline void deactivate() { m_active = false; }

	inline unsigned int lastMoveTime() const { return m_lastMoveTime; }
	inline void lastMoveTime(unsigned int time) { m_lastMoveTime = time; }

	inline unsigned int nextExperience() const { return m_nextExperience; }
	inline void nextExperience(unsigned int nextExperience) { m_nextExperience = nextExperience; }

	inline const unsigned char* preview() const { return m_preview; }
	void calculatePreview();

	void pose(characterAction_e::type_t actionId);
	inline unsigned char pose() { return m_pose; }

	bool teleportEffect() { return m_hasTeleportEffect; }
	void activateTeleportEffect() { m_hasTeleportEffect = true; }
	void deactivateTeleportEffect() { m_hasTeleportEffect = false; }

	const std::string& name() const { return m_attributes.m_name; }

	unsigned char mapId() const { return m_attributes.m_mapId; }
	void mapId(unsigned char mapId) { m_attributes.m_mapId = mapId; }

	const eMUShared::position_t& position() const { return m_attributes.m_position; }
	void position(const eMUShared::position_t &pos) { m_attributes.m_position = pos; }

	unsigned char direction() const { return m_attributes.m_direction; }
	void direction(unsigned char direction) { m_attributes.m_direction = direction; }

	unsigned short viewRange() const { return m_viewRange; }
	void viewRange(unsigned char viewRange) { m_viewRange = viewRange; }

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