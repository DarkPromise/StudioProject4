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

	// Set and Get Functions
	void addTile(Mesh * mesh);
	Mesh * getMesh(int index = 0);

	void setTileSize(int tileSize);
	int getTileSize();
private:
	int m_iTileSize;

	BoundingBox m_cGridAABB;
	std::vector<Mesh*> m_cGridTiles;
};

#endif