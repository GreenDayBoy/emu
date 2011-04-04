#ifndef eMU_GAMESERVER_ITEM_H
#define eMU_GAMESERVER_ITEM_H

#include <map>

class itemAttributesManager_t {
public:
	struct itemAttributes_t {
		unsigned int m_id;
		int m_slot;
		bool m_hasSkill;
		unsigned char m_x;
		unsigned char m_y;
		bool m_hasSerial;
		bool m_hasOption;
		bool m_canDrop;
		std::string m_name;
		unsigned short m_dropLevel;
		unsigned int m_damageMin;
		unsigned int m_damageMax;
		unsigned int m_attackSpeed;
		unsigned char m_durability;
		unsigned char m_magicDurability;
		unsigned int m_magicDamage;
		unsigned short m_requiredLevel;
		unsigned short m_requiredStrength;
		unsigned short m_requiredAgility;
		unsigned short m_requiredEnergy;
		unsigned short m_requiredVitality;
		unsigned short m_requiredCommand;
		bool m_hasSetAttribute;
		bool m_canWizardEquip;
		bool m_canKnightEquip;
		bool m_canElfEquip;
		bool m_canGladiatorEquip;
		bool m_canLordEquip;
	};

	typedef std::map<int, itemAttributes_t> itemAttributesList_t;

	void startup(const std::string &fileName);
	const itemAttributes_t& getAttributes(int group, int id);

private:
	std::map<int, itemAttributesList_t> m_itemList;
};

class item_t {
public:
	item_t(const itemAttributesManager_t::itemAttributes_t &attr):
	  m_attributes(attr) {}

	const itemAttributesManager_t::itemAttributes_t& getAttributes() const { return m_attributes; }

private:
	const itemAttributesManager_t::itemAttributes_t &m_attributes;
};

#endif // eMU_GAMESERVER_ITEM_H