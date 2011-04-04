#ifndef eMU_GAMESERVER_GAMEOBJECT_H
#define eMU_GAMESERVER_GAMEOBJECT_H

#include <string>
#include "viewport.h"
#include "enum.h"
#include "..\shared\shared.h"

#pragma warning(disable: 4355)

class gameObject_t {
public:
	typedef std::list<gameObject_t*> gameObjectList_t;

	gameObject_t(gameObject_e::type_t type);
	virtual ~gameObject_t() {}

	friend std::ostream& operator<<(std::ostream &out, const gameObject_t &object) {
		out << "[" << object.getName() << "]";
		return out;
	}

	virtual void reset();

	gameObject_e::type_t getType() const { return m_type; }

	unsigned int calculateDistance(const gameObject_t &object) const;

	viewportManager_t::viewport_t& getViewport() { return m_viewport; }

	virtual const std::string& getName() const = 0;

	virtual unsigned short getViewRange() const = 0;

	virtual unsigned char getMapId() const = 0;

	virtual const eMUShared::position_t& getPosition() const = 0;

	virtual unsigned char getDirection() const = 0;

protected:
	gameObject_e::type_t m_type;
	viewportManager_t::viewport_t	m_viewport;
};

#pragma warning(default: 4355)

#endif // eMU_GAMESERVER_GAMEOBJECT_H