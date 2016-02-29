#ifndef GRID_H
#define GRID_H

#include <string>
#include <iostream>
#include <vector>

#include "AxisAlignedBoundingBox.h"
#include "MeshBuilder.h"

class Entity;

class GridMap;

class Grid
{
public:
	enum TILE_TYPE
	{
		TILE_FLOOR = 1,
		TILE_WALL = 2,
		TILE_DOOR_CLEAR = 3,
		TILE_BOX = 4,
		TILE_SWITCH = 5,
		TILE_SWITCH_ACTIVATED = 6,
	};


	Grid(int tileSize);
	~Grid();

	// Set and Get Functions
	void addTile(int tileID);
	void replaceTile(int tileID, int index = 0);
	void replaceMesh(int tileID, int index = 0);
	Mesh * getMesh(int index = 0);
	std::vector<Mesh*> getGridTiles();

	void setTileSize(int tileSize);
	int getTileSize();

	void setTileID(int ID);
	int getTileID();

	void setGridPos(Vector3 gridPos);
	Vector3 getGridPos();

	void setremoveDoor(bool removeDoor);
	bool getremoveDoor();
public:
	void addGridEntity(Entity * gridEntity);
	Entity * getGridEntity();
	void removeEntity();
	void deleteEntity();
	bool hasInteractableEntity();
	int getGridEntityType();
	void toggleObjects(GridMap * currMap);
private:
	int m_iTileSize;
	int m_iTileID;
	BoundingBox m_cGridAABB;
	std::vector<Mesh*> m_cGridTiles;
	Vector3 m_v3GridPos;
	Entity * m_cGridEntity;
	bool removeDoor;
};

#endif