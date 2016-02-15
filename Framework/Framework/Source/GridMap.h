#ifndef GRID_MAP_H
#define GRID_MAP_H

#include <vector>

#include "View.h"

#include "Grid.h"

class GridMap
{
public:
	GridMap();
	GridMap(int xSize, int ySize, int tileSize = 32);
	~GridMap();

	// Generic Functions
	void Init(int xSize, int ySize, int tileSize = 32);

	// Rendering Function
	void RenderGrids(View * theView, bool renderBB = false);

private:
	std::vector<std::vector<Grid*>> m_cGridMap;
};

#endif