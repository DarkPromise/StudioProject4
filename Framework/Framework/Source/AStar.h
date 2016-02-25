#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <vector>

#include "GridMap.h"

#define MIN_COST (float) 1.0				// Minimum Permissible Cost	For Heuristics	
#define ALPHA    (float) 0.5				// Scaling Factor For Heuristics
#define abs(x) (((x) >= 0.0) ? (x) :-(x))	// Customised Macro To Absolute Value Conversion

struct AStarNode
{
public:
	AStarNode();
	AStarNode * parentNode; 

	int x;
	int y;

	float f;
	float g;
	float h;
};

class AStar
{
public:
	AStar(int sX, int sY, int gX, int gY, GridMap * gridMap);

	bool StartPathfinding();

	float calculateH(AStarNode*node);
	float calculateG(AStarNode*node);

	void createBestPath(AStarNode * node);
	AStarNode * getBestNode();

	AStarNode * getNeighbourNode(AStarNode * node, int direction);

	void removeFromOpenList(AStarNode * node);
	void removeFromCloseList(AStarNode * node);
	bool isInsideList(std::vector<AStarNode*> nodeList, AStarNode * node);
	AStarNode * getFromOpenList(AStarNode * node);
	AStarNode * getFromCloseList(AStarNode * node);
	void showList(std::vector<AStarNode*> nodeList);
public:
	AStarNode * startNode;
	AStarNode * goalNode;
	std::vector<AStarNode*> openList;
	std::vector<AStarNode*> closeList;
	std::vector<Vector3> bestPath;
	GridMap * gridMap;
};

#endif