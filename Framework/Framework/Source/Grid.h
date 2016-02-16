#ifndef GRID_H
#define GRID_H

#include <string>
#include <iostream>
#include <vector>

#include "AxisAlignedBoundingBox.h"
#include "MeshBuilder.h"

class Grid
{
public:
	Grid(int tileSize);
	~Grid();

	enum TILE_TYPE
	{
		TILE_FLOOR = 1,
		TILE_WALL = 2,
	};

	// Set and Get Functions
	void addTile(int tileID);
	Mesh * getMesh(int index = 0);
	std::vector<Mesh*> getGridTiles();

	void setTileSize(int tileSize);
	int getTileSize();

	void setTileID(int ID);
	int getTileID();

	void setGridPos(Vector3 gridPos);
	Vector3 getGridPos();

private:
	int m_iTileSize;
	int m_iTileID;
	BoundingBox m_cGridAABB;
	std::vector<Mesh*> m_cGridTiles;
	Vector3 m_v3GridPos;
};

#endif