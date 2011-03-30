#include <math.h>
#include "gameobject.h"

gameObject_t::gameObject_t(gameObjectType_t type):
  m_type(type) {}

void gameObject_t::reset() {
	m_viewport.clear();
}

unsigned int gameObject_t::calculateDistance(const gameObject_t &object) const {
	if(this->getPosX() == object.getPosX() && this->getPosY() == object.getPosY()) {
		return 0;
	} else {
		unsigned int distanceX = abs(this->getPosX() - object.getPosX());
		unsigned int distanceY = abs(this->getPosY() - object.getPosY());

		return static_cast<unsigned int>(sqrt(static_cast<float>(distanceX * distanceX + distanceY * distanceY)));
	}
}