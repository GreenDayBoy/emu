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
		attr.m_windProtect = attrFile.readFromProperty<unsigned int>("monsterAttributes", "windProtect", 0);
		attr.m_poisonProtect = attrFile.readFromProperty<unsigned int>("monsterAttributes", "poisonProtect", 0);
		attr.m_iceProtect = attrFile.readFromProperty<unsigned int>("monsterAttributes", "iceProtect", 0);	
		attr.m_electricProtect = attrFile.readFromProperty<unsigned int>("monsterAttributes", "electricProtect", 0);
		attr.m_fireProtect = attrFile.readFromProperty<unsigned int>("monsterAttributes", "fireProtect", 0);
		attr.m_minDamage = attrFile.readFromProperty<unsigned int>("monsterAttributes", "minDamage", 0);
		attr.m_maxDamage = attrFile.readFromProperty<unsigned int>("monsterAttributes", "maxDamage", 0);
		attr.m_defense = attrFile.readFromProperty<unsigned int>("monsterAttributes", "defense", 0);

		m_attributesList[monsterId] = attr;
	}
}

monster_t::monster_t(int index):
  gameObject_t(gameObject_t::_OBJECT_MONSTER),
  m_index(index),
  m_id(0),
  m_magicDefense(0),
  m_attackSuccessRate(0),
  m_defenseSuccessRate(0),
  m_attackSpeed(0),
  m_attackType(0),
  m_attackRange(0),
  m_moveRange(0),
  m_moveSpeed(0),
  m_respawnTime(0),
  m_itemDropRate(0),
  m_maxItemDropLevel(0),
  m_moneyDropRate(0),
  m_windProtect(0),
  m_poisonProtect(0),
  m_iceProtect(0),
  m_electricProtect(0),
  m_fireProtect(0),
  m_minDamage(0),
  m_maxDamage(0),
  m_defense(0),
  m_isNpc(false) {}

void monster_t::setAttributes(const monsterAttributesManager_t::monsterAttributes_t &attr) {
	m_name = attr.m_name;
	m_level = attr.m_level;
	m_health = attr.m_maxHealth;
	m_maxHealth = attr.m_maxHealth;
	m_mana = attr.m_maxMana;
	m_maxMana = attr.m_maxMana;
	m_magicDefense = attr.m_magicDefense;
	m_attackSuccessRate = attr.m_attackSuccessRate;
	m_defenseSuccessRate = attr.m_defenseSuccessRate;
	m_attackSpeed = attr.m_attackSpeed;
	m_attackType = attr.m_attackType;
	m_attackRange = attr.m_attackRange;
	m_viewRange = attr.m_viewRange;
	m_moveRange = attr.m_moveRange;
	m_moveSpeed = attr.m_moveSpeed;
	m_respawnTime = attr.m_respawnTime;
	m_itemDropRate = attr.m_itemDropRate;
	m_maxItemDropLevel = attr.m_maxItemDropLevel;
	m_moneyDropRate = attr.m_moneyDropRate;
	m_windProtect = attr.m_windProtect;
	m_poisonProtect = attr.m_poisonProtect;
	m_iceProtect = attr.m_iceProtect;
	m_electricProtect = attr.m_electricProtect;
	m_fireProtect = attr.m_fireProtect;
	m_minDamage = attr.m_minDamage;
	m_maxDamage = attr.m_maxDamage;
	m_defense = attr.m_defense;
}

monsterManager_t::monsterManager_t(monsterAttributesManager_t	&monsterAttributesManager,
									int startIndex):
  m_monsterAttributesManager(monsterAttributesManager),
  m_startIndex(startIndex) {}

monsterManager_t::~monsterManager_t() {
	for(size_t i = 0; i < m_monsterList.size(); ++i) {
		delete m_monsterList[i];
	}
}

void monsterManager_t::startup(const std::string &fileName,
							   const boost::function1<void, monster_t*> &registerObjectCallback) {
	eMUCore::xmlConfig_t monstersFile;
	monstersFile.open(fileName, "monsters");

	int i = m_startIndex;

	while(monstersFile.nextNode()) {
		monster_t *monster = new monster_t(i++);

		monster->setMapId(monstersFile.readFromProperty<unsigned int>("monster", "mapId", 0));
		monster->setPosX(monstersFile.readFromProperty<unsigned int>("monster", "posX", 0));
		monster->setPosY(monstersFile.readFromProperty<unsigned int>("monster", "posY", 0));
		monster->setDirection(monstersFile.readFromProperty<unsigned int>("monster", "direction", 0));
		monster->setId(monstersFile.readFromProperty<int>("monster", "id", 0));
		monster->setAttributes(m_monsterAttributesManager[monster->getId()]);

		m_monsterList.push_back(monster);
		registerObjectCallback(monster);
	}
}