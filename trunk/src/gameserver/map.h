#ifndef eMU_GAMESERVER_MAP_H
#define eMU_GAMESERVER_MAP_H

#include <string>
#include <vector>
#include <map>
#include "enum.h"

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
	typedef std::pair<unsigned char, unsigned char> position_t;
	typedef std::vector<position_t> path_t;

	map_t();

	void startup(const std::string &filename, int mapId);
	bool isPathValid(const path_t &path) const;
	std::string dumpPath(const path_t &path) const;
	position_t getRandomPosition(unsigned char x1 = 0,
									unsigned char y1 = 0,
									unsigned char x2 = 255,
									unsigned char y2 = 255) const;

	inline unsigned char getTileAttribute(unsigned char x, unsigned char y) const {
		return m_mapTiles[(y << 8) + x];
	}

	inline bool canStand(unsigned char x, unsigned char y) const {
		unsigned char attr = this->getTileAttribute(x, y) & 127; // & 127 - without stand bit.
		return (attr == mapTileAttribute_e::_ground || attr == mapTileAttribute_e::_safezone);
	}

	inline bool isTileEmpty(unsigned char x, unsigned char y) const {
		return ((this->getTileAttribute(x, y) & 128) == 0);
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

	void startup(const std::string &filename);
	void cleanup();
	map_t& operator[](int mapId);
	bool isMapExists(int mapId);

private:
	mapManager_t(const mapManager_t&);
	mapManager_t& operator=(const mapManager_t&);

	mapList_t m_mapList;
};

#endif // eMU_GAMESERVER_MAP_H