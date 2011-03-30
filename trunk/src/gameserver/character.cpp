#include "character.h"

character_t::character_t(gameServerUser_t &owner):
  gameObject_t(gameObject_t::_OBJECT_CHARACTER),
  m_owner(owner) {
	__super::reset();
	this->reset();
}

void character_t::setAttributes(const eMUShared::characterAttributes_t &attr) {
	m_attributes = attr;
	m_viewRange = 15;
}

void character_t::reset() {
	__super::reset();

	m_active = false;
	m_pose = 0;
	m_hasTeleportEffect = false;
	m_attributes.clear();
}

void character_t::setPreview() {
	m_preview[0] = ((m_attributes.m_race >> 4) << 5) & 0xE0;
	m_preview[0] |= ((m_attributes.m_race & 0x07) << 4) & 0x10;
	m_preview[0] |= m_pose & 0x0F;
	m_preview[1] = 0x20;
	m_preview[2] = 0xFF;
	m_preview[3] = 0xFF;
	m_preview[4] = 0xFF;
	m_preview[5] = 0xFF;
	m_preview[6] = 0x00;
	m_preview[7] = 0x00;
	m_preview[8] = 0x00;
	m_preview[9] = 0xF8;
	m_preview[10] = 0x00;
	m_preview[11] = 0x00;
	m_preview[12] = 0x20;
	m_preview[13] = 0xFF;
	m_preview[14] = 0xFF;
	m_preview[15] = 0xFF;
	m_preview[16] = 0x00;
	m_preview[17] = 0x00;
}

void character_t::setPose(unsigned char actionId) {
	unsigned char pose = m_pose;

	switch(actionId) {
		case 0x7B: // stoi.
			pose = 0;
			break;

		case 0x80: // siedzi.
			pose = 2;
			break;

		case 0x81: // oparty.
			pose = 3;
			break;

		case 0x82: // wisi
			pose = 4;
			break;
	}

	if(m_pose != 0) {
		m_preview[0] &= ~(m_pose & 0x0F);
	}

	m_preview[0] |= pose & 0x0F;
	m_pose = pose;
}