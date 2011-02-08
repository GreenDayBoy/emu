#ifndef eMU_GAMESERVER_MAP_H
#define eMU_GAMESERVER_MAP_H

#include <string>
#include <vector>
#include <map>

const size_t c_mapWidth = 256;
const size_t c_mapHeight = 256;
const size_t c_mapSize = c_mapWidth * c_mapHeight;

struct mapFileHeader_t {
	unsigned char m_unk;
	unsigned char m_mapWidth;
	unsigned char m_mapHeight;
};

class map_t {
public:
	enum mapTileAttribute_t {
		_ATTR_GROUND = 0,
		_ATTR_SAFEZONE,
		_ATTR_SAFEZONE_NONGROUND = 4,
		_ATTR_NONGROUND
	};

	typedef std::pair<unsigned char, unsigned char> position_t;
	typedef std::vector<position_t> path_t;

	map_t();

	void startup(const std::string &filename, int mapId);

	inline unsigned char getTileAttribute(unsigned char x, unsigned char y) const {
		return m_mapTiles[(y << 8) + x];
	}

	inline bool canStand(unsigned char x, unsigned char y) const {
		unsigned char attr = this->getTileAttribute(x, y) & 127; // & 127 - without stand bit.
		return (attr == _ATTR_GROUND || attr == _ATTR_SAFEZONE);
	}

	inline void setStand(unsigned char x, unsigned char y) { m_mapTiles[(y << 8) + x] |= 128; }
	inline void clearStand(unsigned char x, unsigned char y) { m_mapTiles[(y << 8) + x] &= 127; }

	inline void map_t::resetStand(unsigned char oldX,
									unsigned char oldY,
									unsigned char newX,
									unsigned char newY) {
		this->clearStand(oldX, oldY);
		this->setStand(newX, newY);
	}

	bool isPathValid(const path_t &path) const;
	std::string dumpPath(const path_t &path) const;

private:
	map_t(const map_t&);
	map_t& operator=(const map_t&);

	int m_mapId;
	unsigned char m_mapTiles[c_mapSize];
};

class mapManager_t {
public:
	typedef std::map<int, map_t*> mapList_t;

	mapManager_t() {}
	~mapManager_t();

	void startup(const std::string &filename);
	map_t& operator[](int mapId);
	bool isMapExists(int mapId);

private:
	mapManager_t(const mapManager_t&);
	mapManager_t& operator=(const mapManager_t&);

	mapList_t m_mapList;
};

#endif // eMU_GAMESERVER_MAP_H