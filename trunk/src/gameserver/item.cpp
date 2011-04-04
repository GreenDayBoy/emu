#include "item.h"
#include "..\core\core.h"
#include <iostream>

void itemAttributesManager_t::startup(const std::string &fileName) {
	eMUCore::xmlConfig_t attrFile;
	attrFile.open(fileName, "items");

	while(attrFile.nextNode()) {
		int group = attrFile.readFromProperty<int>("group", "id", -1);

		if(group == -1) {
			eMUCore::exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << "[itemAttributesManager_t::startup()] invalid group or group not defined in file ["
					<< fileName << "].";
			throw e;
		}

		attrFile.childrenNode();

		while(attrFile.nextNode()) {
			itemAttributes_t attr;
			attr.m_id = attrFile.readFromProperty<unsigned int>("item", "id", 0xDEADC0DE);
			attr.m_slot = attrFile.readFromProperty<int>("item", "slot", 0);
			attr.m_hasSkill = attrFile.readBoolFromProperty("item", "skill", false);
			attr.m_x = attrFile.readFromProperty<unsigned int>("item", "x", 0);
			attr.m_y = attrFile.readFromProperty<unsigned int>("item", "y", 0);
			attr.m_hasSerial = attrFile.readBoolFromProperty("item", "serial", false);
			attr.m_hasOption = attrFile.readBoolFromProperty("item", "option", false);
			attr.m_canDrop = attrFile.readBoolFromProperty("item", "drop", false);
			attr.m_name = attrFile.readFromProperty<std::string>("item", "name", "default");
			attr.m_dropLevel = attrFile.readFromProperty<unsigned short>("item", "dropLevel", 0);
			attr.m_damageMin = attrFile.readFromProperty<unsigned int>("item", "damageMin", 0);
			attr.m_damageMax = attrFile.readFromProperty<unsigned int>("item", "damageMax", 0);
			attr.m_attackSpeed = attrFile.readFromProperty<unsigned int>("item", "attackSpeed", 0);
			attr.m_durability = attrFile.readFromProperty<unsigned int>("item", "durability", 0);
			attr.m_magicDurability = attrFile.readFromProperty<unsigned int>("item", "magicDurability", 0);
			attr.m_magicDamage = attrFile.readFromProperty<unsigned int>("item", "magicDamage", 0);
			attr.m_requiredLevel = attrFile.readFromProperty<unsigned short>("item", "reqLevel", 0);
			attr.m_requiredStrength = attrFile.readFromProperty<unsigned int>("item", "reqStrength", 0);
			attr.m_requiredAgility = attrFile.readFromProperty<unsigned int>("item", "reqAgility", 0);
			attr.m_requiredEnergy = attrFile.readFromProperty<unsigned int>("item", "reqEnergy", 0);
			attr.m_requiredVitality = attrFile.readFromProperty<unsigned int>("item", "reqVitality", 0);
			attr.m_requiredCommand = attrFile.readFromProperty<unsigned int>("item", "reqCommand", 0);
			attr.m_hasSetAttribute = attrFile.readBoolFromProperty("item", "setAttribute", false);
			attr.m_canWizardEquip = attrFile.readBoolFromProperty("item", "eqWizard", false);
			attr.m_canKnightEquip = attrFile.readBoolFromProperty("item", "eqKnight", false);
			attr.m_canElfEquip = attrFile.readBoolFromProperty("item", "eqElf", false);
			attr.m_canGladiatorEquip = attrFile.readBoolFromProperty("item", "eqGladiator", false);
			attr.m_canLordEquip = attrFile.readBoolFromProperty("item", "eqLord", false);

			if(attr.m_id != 0xDEADC0DE) {
				m_itemList[group][attr.m_id] = attr;
			} else {
				eMUCore::exception_t e;
				e.in() << __FILE__ << ":" << __LINE__ << "[itemAttributesManager_t::getAttributes()] Invalid item id or item id not defined in group ["
					<< group << "].";
				throw e;
			}
		}

		attrFile.parentNode();
	}

	attrFile.close();
}

const itemAttributesManager_t::itemAttributes_t& itemAttributesManager_t::getAttributes(int group, int id) {
	if(m_itemList.find(group) != m_itemList.end()) {
		itemAttributesList_t &list = m_itemList[group];

		if(list.find(id) != list.end()) {
			return list[id];
		} else {
			eMUCore::exception_t e;
			e.in() << __FILE__ << ":" << __LINE__ << "[itemAttributesManager_t::getAttributes()] No item in group ["
				<< group << "] with id [" << id << "].";
			throw e;
		}
	} else {
		eMUCore::exception_t e;
		e.in() << __FILE__ << ":" << __LINE__ << "[itemAttributesManager_t::getAttributes()] No items list for group ["
			<< group << "].";
		throw e;
	}
}