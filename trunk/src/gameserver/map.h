#ifndef eMU_GAMESERVER_MAP_H
#define eMU_GAMESERVER_MAP_H

#include <string>
#include <vector>
#include <map>
#include "..\shared\shared.h"
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
	typedef std::vector<eMUShared::position_t> path_t;

	map_t();

	void startup(const std::string &filename, int mapId);
	bool pathValid(const path_t &path) const;
	std::string dumpPath(const path_t &path) const;
	eMUShared::position_t randomPosition(const eMUShared::position_t &startPos = eMUShared::position_t(0, 0),
											const eMUShared::position_t &endPos = eMUShared::position_t(255, 255)) const;

	inline unsigned char tileAttribute(const eMUShared::position_t &pos) const {
		return m_mapTiles[(pos.m_y << 8) + pos.m_x];
	}

	inline bool canStand(const eMUShared::position_t &pos) const {
		unsigned char attr = this->tileAttribute(pos) & 127; // & 127 - without stand bit.
		return (attr == mapTileAttribute_e::_ground || attr == mapTileAttribute_e::_safezone);
	}

	inline bool tileEmpty(const eMUShared::position_t &pos) const {
		return ((this->tileAttribute(pos) & 128) == 0);
	}

	inline void stand(const eMUShared::position_t &pos) { m_mapTiles[(pos.m_y << 8) + pos.m_x] |= 128; }
	inline void clearStand(const eMUShared::position_t &pos) { m_mapTiles[(pos.m_y << 8) + pos.m_x] &= 127; }
	inline void map_t::resetStand(const eMUShared::position_t &oldPos,
									const eMUShared::position_t &newPos) {
		this->clearStand(oldPos);
		this->stand(newPos);
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
	bool mapExists(int mapId);

private:
	mapManager_t(const mapManager_t&);
	mapManager_t& operator=(const mapManager_t&);

	mapList_t m_mapList;
};

#endif // eMU_GAMESERVER_MAP_H