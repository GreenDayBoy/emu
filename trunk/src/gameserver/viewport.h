#ifndef GAMESERVER_VIEWPORT_H
#define GAMESERVER_VIEWPORT_H

#include <list>
#include <set>
#include "..\core\core.h"

class gameObject_t; // definition in gameobject.h.
class protocol_t; // definition in protocol.h.

class viewportManager_t {
public:
	typedef std::set<gameObject_t*> viewport_t;

	viewportManager_t(protocol_t &protocol);

	void registerObject(gameObject_t *object) { m_objectList.push_back(object); }
	void unregisterObject(gameObject_t *object) { m_objectList.remove(object); }

	void generate(gameObject_t &object);
	void update(gameObject_t &object);
	void clear(gameObject_t &object);

private:
	viewportManager_t();
	viewportManager_t(const viewportManager_t&);
	viewportManager_t& operator=(const viewportManager_t&);

	std::list<gameObject_t*>	m_objectList;
	protocol_t					&m_protocol;
};

#endif GAMESERVER_VIEWPORT_H