#include "character.h"

character_t::character_t(gameServerUser_t &owner):
  gameObject_t(gameObject_t::_OBJECT_CHARACTER),
  m_owner(owner) {
	gameObject_t::reset();
	this->reset();
}

void character_t::setAttributes(const eMUShared::characterAttributes_t &attr) {
	m_name = attr.m_name;
	m_race = attr.m_race;
	m_posX = attr.m_posX;
	m_posY = attr.m_posY;
	m_mapId = attr.m_mapId;
	m_direction = attr.m_direction;
	m_experience = attr.m_experience;
	m_levelUpPoints = attr.m_levelUpPoints;
	m_level = attr.m_level;
	m_strength = attr.m_strength;
	m_agility = attr.m_agility;
	m_vitality = attr.m_vitality;
	m_energy = attr.m_energy;
	m_health = attr.m_health;
	m_maxHealth = attr.m_maxHealth;
	m_mana = attr.m_mana;
	m_maxMana = attr.m_maxMana;
	m_shield = attr.m_shield;
	m_maxShield = attr.m_maxShield;
	m_stamina = attr.m_stamina;
	m_maxStamina = attr.m_maxStamina;
	m_money = attr.m_money;
	m_pkLevel = attr.m_pkLevel;
	m_controlCode = attr.m_controlCode;
	m_addPoints = attr.m_addPoints;
	m_maxAddPoints = attr.m_maxAddPoints;
	m_command = attr.m_command;
	m_minusPoints = attr.m_minusPoints;
	m_maxMinusPoints = attr.m_maxMinusPoints;
	m_viewRange = 15;
}

void character_t::reset() {
	m_active = false;
	m_name = "";
	m_race = 0;
	m_posX = 0;
	m_posY = 0;
	m_mapId = 0;
	m_lastMoveTime = 0;
	m_direction = 0;
	m_experience = 0;
	m_nextExperience = 0;
	m_levelUpPoints = 0;
	m_level = 0;
	m_strength = 0;
	m_agility = 0;
	m_vitality = 0;
	m_energy = 0;
	m_health = 0;
	m_maxHealth = 0;
	m_mana = 0;
	m_maxMana = 0;
	m_shield = 0;
	m_maxShield = 0;
	m_stamina = 0;
	m_maxStamina = 0;
	m_money = 0;
	m_pkLevel = 0;
	m_controlCode = 0;
	m_addPoints = 0;
	m_maxAddPoints = 0;
	m_command = 0;
	m_minusPoints = 0;
	m_maxMinusPoints = 0;
	m_pose = 0;
	m_teleportEffect = false;

	gameObject_t::reset();
}

void character_t::setPreview() {
	m_preview[0] = ((m_race >> 4) << 5) & 0xE0;
	m_preview[0] |= ((m_race & 0x07) << 4) & 0x10;
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