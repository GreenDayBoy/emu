#ifndef eMU_GAMESERVER_GAMEOBJECT_H
#define eMU_GAMESERVER_GAMEOBJECT_H

#include <string>
#include "viewport.h"

#pragma warning(disable: 4355)

class gameObject_t {
public:
	enum gameObjectType_t {
		_OBJECT_CHARACTER = 0xE1,
		_OBJECT_MONSTER,
	};

	typedef std::list<gameObject_t*> gameObjectList_t;

	gameObject_t(gameObjectType_t type);
	virtual ~gameObject_t() {}

	friend std::ostream& operator<<(std::ostream &out, const gameObject_t &object) {
		out << "[" << object.getName() << "]";
		return out;
	}

	gameObjectType_t getType() const { return m_type; }
	unsigned int calculateDistance(const gameObject_t &object) const;
	viewportManager_t::viewport_t& getViewport() { return m_viewport; }
	virtual void reset();

	virtual const std::string& getName() const = 0;
	virtual unsigned short getLevel() const = 0;
	virtual unsigned int getMaxHealth() const = 0;
	virtual unsigned int getMaxMana() const = 0;
	virtual unsigned short getViewRange() const = 0;

	virtual unsigned int getHealth() const = 0;
	virtual void setHealth(unsigned short health) = 0;

	virtual unsigned int getMana() const = 0;
	virtual void setMana(unsigned short mana) = 0;

	virtual unsigned char getMapId() const = 0;
	virtual void setMapId(unsigned char mapId) = 0;

	virtual unsigned char getPosX() const = 0;
	virtual void setPosX(unsigned char posX) = 0;

	virtual unsigned char getPosY() const = 0;
	virtual void setPosY(unsigned char posY) = 0;

	virtual unsigned char getDirection() const = 0;
	virtual void setDirection(unsigned char direction) = 0;

protected:
	gameObjectType_t m_type;
	viewportManager_t::viewport_t	m_viewport;
};

#pragma warning(default: 4355)

#endif // eMU_GAMESERVER_GAMEOBJECT_H