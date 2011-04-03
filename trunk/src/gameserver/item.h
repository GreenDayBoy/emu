#ifndef eMU_GAMESERVER_ITEM_H
#define eMU_GAMESERVER_ITEM_H

#include <map>

class itemAttributesManager_t {
public:
	struct itemAttributes_t {

	};

	typedef std::map<int, itemAttributes_t> itemAttributesList_t;

	void startup(const std::string &fileName);
	const itemAttributes_t& getAttributes(int group, int id);

private:
	std::map<int, itemAttributesList_t> m_itemList;
};

#endif // eMU_GAMESERVER_ITEM_H