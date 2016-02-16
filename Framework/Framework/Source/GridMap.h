#ifndef GRID_MAP_H
#define GRID_MAP_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "View.h"

#include "Grid.h"

#define BACKGROUND_TILES 0
#define FOREGROUND_TILES 1

class GridMap
{
public:
	GridMap();
	GridMap(int xSize, int ySize, int tileSize = 32);
	virtual ~GridMap();

	// Generic Functions
	void Init(int xSize, int ySize, int tileSize = 32);
	bool LoadData(std::string backgroundCSV, std::string foregroundCSV);
	virtual void Update(double dt);

	// Rendering Function
	void RenderGrids(View * theView, bool renderBB = false);

	// Setters and Getters
	std::vector<std::vector<Grid*>> getGridMap();
	std::vector<Mesh*> getTileMap();

public:
	std::vector<std::vector<int> > backgroundData;	// 2D vector to store background values
	std::vector<std::vector<int> > foregroundData;	// 2D vector to store foreground values
	std::vector<std::vector<int> > collisionData;	// 2D vector to store collision values
private:
	bool LoadBackgroundMap(const std::string fileName);	// Loads values from background csv into vector
	bool LoadForegroundMap(const std::string fileName);	// Loads values from foreground csv into vector
private:
	std::vector<std::vector<Grid*>> m_cGridMap;
	std::vector<Mesh*> m_cTileMap;

	int m_iTileSize;
	int m_iNumTilesWidth;
	int m_iNumTilesHeight;
};

#endif