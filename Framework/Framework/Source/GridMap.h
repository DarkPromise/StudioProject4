#ifndef GRID_MAP_H
#define GRID_MAP_H

#pragma warning(disable:4244)
#pragma warning(disable:4018)

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "View.h"

#include "Grid.h"

#define BOUNDING_BOX 0
#define BACKGROUND_TILE 1
#define FOREGROUND_TILE 2

class GridMap
{
public:
	enum GRID_DIRECTION
	{
		DIRECTION_UP = 0,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
		DIRECTION_NONE,
	};

	GridMap();
	GridMap(int xSize, int ySize, int tileSize = 32);
	virtual ~GridMap();

	// Generic Functions
	void Init(int xSize, int ySize, int tileSize = 32);
	bool LoadData(std::string backgroundCSV);
	void ResetData();
	virtual void Update(double dt);

	// Rendering Function
	void RenderGrids(View * theView, Mesh * textMesh, bool renderBB = false);
	void RenderLevel(View * theView);
	void RenderForeground(View * theView);
	void RenderGridEntities(View * theView);

	// Setters and Getters
	std::vector<std::vector<Grid*>> getGridMap();
	std::vector<Mesh*> getTileMap();
	void setTileSize(int tileSize);
	int getTileSize();
	void setMapWidth(int width);
	int getMapWidth();
	void setMapHeight(int height);
	int getMapHeight();

	//Special Functions
	bool PushObjects(int pIndexX, int pIndexY, int direction, int EntityType, Entity * Player);
	void addGridEntity(Entity * entity);
	void removeGridEntity(Entity * entity);
public:
	std::vector<std::vector<int> > backgroundData;	// 2D vector to store background values
	std::vector<std::vector<int> > foregroundData;	// 2D vector to store foreground values
	std::vector<std::vector<int> > collisionData;	// 2D vector to store collision values
private:
	bool LoadBackgroundMap(const std::string fileName);	// Loads values from background csv into vector
	bool LoadForegroundMap(const std::string fileName);	// Loads values from foreground csv into vector
private:
	std::vector<std::vector<Grid*>> m_cGridMap;

	int m_iTileSize;
	int m_iNumTilesWidth;
	int m_iNumTilesHeight;
};

#endif