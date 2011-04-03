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