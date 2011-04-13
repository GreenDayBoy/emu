#include <math.h>
#include "gameobject.h"

gameObject_t::gameObject_t(gameObject_e::type_t type):
  m_type(type) {}

void gameObject_t::reset() {
	m_viewport.clear();
}

unsigned int gameObject_t::calculateDistance(const gameObject_t &object) const {
	if(this->position().m_x == object.position().m_x && this->position().m_y == object.position().m_y) {
		return 0;
	} else {
		unsigned int distanceX = abs(this->position().m_x - object.position().m_x);
		unsigned int distanceY = abs(this->position().m_y - object.position().m_y);

		return static_cast<unsigned int>(sqrt(static_cast<float>(distanceX * distanceX + distanceY * distanceY)));
	}
}