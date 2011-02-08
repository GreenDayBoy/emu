#include <math.h>
#include "gameobject.h"

gameObject_t::gameObject_t(gameObjectType_t type):
  m_type(type),
  m_level(0),
  m_health(0),
  m_maxHealth(0),
  m_mana(0),
  m_maxMana(0),
  m_mapId(0),
  m_posX(0),
  m_posY(0),
  m_direction(0),
  m_viewRange(0) {}

void gameObject_t::reset() {
	m_mapId = 0;
	m_posX = 0;
	m_posY = 0;
	m_direction = 0;
	m_viewRange = 0;
}

unsigned int gameObject_t::calculateDistance(const gameObject_t &object) const {
	if(m_posX == object.getPosX() && m_posY == object.getPosY()) {
		return 0;
	} else {
		unsigned int distanceX = abs(m_posX - object.getPosX());
		unsigned int distanceY = abs(m_posY - object.getPosY());

		return static_cast<unsigned int>(sqrt(static_cast<float>(distanceX * distanceX + distanceY * distanceY)));
	}
}