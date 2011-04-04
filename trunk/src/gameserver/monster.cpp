#include "monster.h"
#include "..\core\core.h"

const monsterAttributesManager_t::monsterAttributes_t& monsterAttributesManager_t::operator[](int monsterId) {
	if(m_attributesList.find(monsterId) != m_attributesList.end()) {
		return m_attributesList[monsterId];
	} else {
		eMUCore::exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << "[monsterAttributesManager_t::operator[]()] No monster attributes for monster ["
				<< monsterId << "].";
		throw e;
	}
}

void monsterAttributesManager_t::startup(const std::string &fileName) {
	eMUCore::xmlConfig_t attrFile;
	attrFile.open(fileName, "monstersAttributes");

	while(attrFile.nextNode()) {
		int monsterId = attrFile.readFromProperty<int>("monsterAttributes", "id", -1);

		monsterAttributes_t attr;
		attr.m_name = attrFile.readFromProperty<std::string>("monsterAttributes", "name", "default");
		attr.m_level = attrFile.readFromProperty<unsigned short>("monsterAttributes", "level", 0);
		attr.m_maxHealth = attrFile.readFromProperty<unsigned int>("monsterAttributes", "maxHealth", 0);
		attr.m_maxMana = attrFile.readFromProperty<unsigned int>("monsterAttributes", "maxMana", 0);
		attr.m_magicDefense = attrFile.readFromProperty<unsigned int>("monsterAttributes", "magicDefense", 0);
		attr.m_attackSuccessRate = attrFile.readFromProperty<unsigned int>("monsterAttributes", "attackSuccessRate", 0);
		attr.m_defenseSuccessRate = attrFile.readFromProperty<unsigned int>("monsterAttributes", "defenseSuccessRate", 0);
		attr.m_attackSpeed = attrFile.readFromProperty<unsigned int>("monsterAttributes", "attackSpeed", 0);
		attr.m_attackType = attrFile.readFromProperty<unsigned int>("monsterAttributes", "attackType", 0);
		attr.m_attackRange = attrFile.readFromProperty<unsigned short>("monsterAttributes", "attackRange", 0);
		attr.m_viewRange = attrFile.readFromProperty<unsigned short>("monsterAttributes", "viewRange", 0);
		attr.m_moveRange = attrFile.readFromProperty<unsigned short>("monsterAttributes", "moveRange", 0);
		attr.m_moveSpeed = attrFile.readFromProperty<unsigned int>("monsterAttributes", "moveSpeed", 0);
		attr.m_respawnTime = attrFile.readFromProperty<unsigned int>("monsterAttributes", "respawnTime", 0);
		attr.m_itemDropRate = attrFile.readFromProperty<unsigned int>("monsterAttributes", "itemDropRate", 0);
		attr.m_maxItemDropLevel = attrFile.readFromProperty<unsigned int>("monsterAttributes", "maxItemDropLevel", 0);
		attr.m_moneyDropRate = attrFile.readFromProperty<unsigned int>("monsterAttributes", "moneyDropRate", 0);
		attr.m_windProtect = attrFile.readBoolFromProperty("monsterAttributes", "windProtect", false);
		attr.m_poisonProtect = attrFile.readBoolFromProperty("monsterAttributes", "poisonProtect", false);
		attr.m_iceProtect = attrFile.readBoolFromProperty("monsterAttributes", "iceProtect", false);	
		attr.m_electricProtect = attrFile.readBoolFromProperty("monsterAttributes", "electricProtect", false);
		attr.m_fireProtect = attrFile.readBoolFromProperty("monsterAttributes", "fireProtect", false);
		attr.m_minDamage = attrFile.readFromProperty<unsigned int>("monsterAttributes", "minDamage", 0);
		attr.m_maxDamage = attrFile.readFromProperty<unsigned int>("monsterAttributes", "maxDamage", 0);
		attr.m_defense = attrFile.readFromProperty<unsigned int>("monsterAttributes", "defense", 0);

		m_attributesList[monsterId] = attr;
	}

	attrFile.close();
}

monster_t::monster_t(int index,
					 const monsterAttributesManager_t::monsterAttributes_t &attr):
  gameObject_t(gameObject_e::_monster),
  m_attributes(attr),
  m_index(index),
  m_id(0),
  m_mapId(0),
  m_direction(0),
  m_health(0),
  m_mana(0) {
	  m_position.clear();
  }

monsterManager_t::monsterManager_t(monsterAttributesManager_t &monsterAttributesManager,
									int startIndex):
  m_monsterAttributesManager(monsterAttributesManager),
  m_startIndex(startIndex) {}

void monsterManager_t::startup(const std::string &fileName,
							   const boost::function1<void, monster_t*> &registerObjectCallback) {
	eMUCore::xmlConfig_t monstersFile;
	monstersFile.open(fileName, "monsters");

	int i = m_startIndex;

	while(monstersFile.nextNode()) {
		int id = monstersFile.readFromProperty<int>("monster", "id", 0);

		monster_t *monster = new monster_t(++i, m_monsterAttributesManager[id]);
		monster->setMapId(monstersFile.readFromProperty<unsigned int>("monster", "mapId", 0));
		monster->setPosition(eMUShared::position_t(monstersFile.readFromProperty<unsigned int>("monster", "posX", 0),
													monstersFile.readFromProperty<unsigned int>("monster", "posY", 0)));
		monster->setDirection(monstersFile.readFromProperty<unsigned int>("monster", "direction", 0));
		monster->setId(id);

		m_monsterList.push_back(monster);
		registerObjectCallback(monster);
	}

	monstersFile.close();
}

void monsterManager_t::cleanup() {
	for(size_t i = 0; i < m_monsterList.size(); ++i) {
		delete m_monsterList[i];
	}
}