#include "AStar.h"

AStarNode::AStarNode()
: parentNode(NULL)
, f(0.f)
, g(0.f)
, h(0.f)
{

}

AStar::AStar(int sX, int sY, int gX, int gY, GridMap * gridMap)
{
	this->startNode = new AStarNode();
	this->goalNode = new AStarNode();
	startNode->x = sX;
	startNode->y = sY;
	goalNode->x = gX;
	goalNode->y = gY;

	startNode->h = this->calculateH(startNode);
	startNode->f = startNode->g + startNode->h;

	this->gridMap = gridMap;
}

bool AStar::StartPathfinding()
{
	AStarNode * tempNode;
	this->openList.push_back(startNode);
	while (!openList.empty())
	{
		AStarNode * bestNode = this->getBestNode();
		if ((bestNode->x == goalNode->x) && (bestNode->y == goalNode->y))
		{
			this->createBestPath(bestNode);
			return true;
		}
		else
		{
			this->removeFromOpenList(bestNode);
			this->closeList.push_back(bestNode);
			AStarNode * newNode;
			for (int i = 0; i < 4; i++)
			{
				newNode = this->getNeighbourNode(bestNode, i);
				if (newNode != NULL)
				{
					newNode->parentNode = bestNode;
					newNode->g = this->calculateG(newNode);
					newNode->h = this->calculateH(newNode);
					newNode->f = newNode->g + newNode->h;
					if (this->isInsideList(openList, newNode))
					{
						tempNode = this->getFromOpenList(newNode);
						if (tempNode->f > newNode->f)
						{
							this->removeFromOpenList(tempNode);
							this->openList.push_back(newNode);
						}
					}
					else if (this->isInsideList(closeList,newNode))
					{
						tempNode = this->getFromCloseList(newNode);
						if (tempNode->f > newNode->f)
						{
							this->removeFromCloseList(tempNode);
							this->closeList.push_back(newNode);
						}
					}
					else
					{
						this->openList.push_back(newNode);
					}
				}
			}
		}
	}
	return false;
}

AStarNode * AStar::getFromOpenList(AStarNode * node)
{
	AStarNode * tempNode = NULL;
	for (int i = 0; i < openList.size(); i++)	
	{
		if ((openList[i]->x == node->x) && (openList[i]->y == node->y))
		{
			tempNode = openList[i];	
			break;
		}
	}
	return tempNode;
}

AStarNode * AStar::getFromCloseList(AStarNode * node)
{
	AStarNode * tempNode = NULL;
	for (int i = 0; i < closeList.size(); i++)		
	{
		if ((closeList[i]->x == node->x) && (closeList[i]->y == node->y))
		{
			tempNode = closeList[i];
			break;
		}
	}
	return tempNode;
}

bool AStar::isInsideList(std::vector<AStarNode*> nodeList, AStarNode * node)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		if ((nodeList[i]->x == node->x) && (nodeList[i]->y == node->y))
		{
			return true;
		}
	}
	return false;
}

AStarNode * AStar::getNeighbourNode(AStarNode * node, int direction)
{
	AStarNode * neighbourNode = NULL;
	Vector3 currIndex = Vector3(node->x, node->y);
	Vector3 dirIndex;
	switch (direction)
	{
	case 0: // Up
		dirIndex = Vector3(0, -1);
		break;
	case 1: // Down
		dirIndex = Vector3(0, 1);
		break;
	case 2: // Right
		dirIndex = Vector3(1, 0);
		break;
	case 3: // Left
		dirIndex = Vector3(-1, 0);
		break; 
	}
	Vector3 neighbourIndex = currIndex + dirIndex;
	if (this->gridMap->getGridMap()[neighbourIndex.y][neighbourIndex.x]->getTileID() == Grid::TILE_FLOOR)
	{
		if (this->gridMap->getGridMap()[neighbourIndex.y][neighbourIndex.x]->getGridEntity() == NULL)
		{
			//Base on direction check for entities
			neighbourNode = new AStarNode();
			neighbourNode->x = neighbourIndex.x;
			neighbourNode->y = neighbourIndex.y;
		}
	}
	return neighbourNode;
}

void AStar::removeFromOpenList(AStarNode * node)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if ((openList[i]->x == node->x) && (openList[i]->y == node->y))
		{
			openList.erase(openList.begin() + i, openList.begin() + i + 1);
		}
	}
}

void AStar::removeFromCloseList(AStarNode * node)
{
	for (int i = 0; i < closeList.size(); i++)
	{
		if ((closeList[i]->x == node->x) && (closeList[i]->y == node->y))
		{
			closeList.erase(closeList.begin() + i, closeList.begin() + i + 1);
		}
	}
}

void AStar::createBestPath(AStarNode * node)
{
	if (node->x == openList[0]->x && node->y == openList[0]->y)
	{
		return;
	}

	node = node->parentNode;
	//this->bestPath.push_back(Vector3(goalNode->y,goalNode->x));	// Goal Indexes
	while (node->parentNode != NULL)
	{
		this->bestPath.push_back(Vector3(node->x, node->y));
		node = node->parentNode;
	}
	//this->bestPath.push_back(Vector3(startNode->y,startNode->x));	// Start Indexes

	/*std::cout << std::endl;
	for (int y = 0; y < gridMap->backgroundData.size(); y++)
	{
		for (int x = 0; x <gridMap->backgroundData[y].size(); x++)
		{
			std::cout << "[" << gridMap->getGridMap()[y][x]->getTileID() << "]";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/
}

AStarNode * AStar::getBestNode()
{
	int bestIndex = 0;
	for (int i = 1; i < openList.size(); i++)
	{
		if (openList[i]->f < openList[bestIndex]->f)
		{
			bestIndex = i;
		}
	}
	return openList[bestIndex];
}

float AStar::calculateG(AStarNode * node)
{
	float tempG;
	if (node->parentNode != NULL)
	{
		tempG = node->parentNode->g;
	}
	else
	{
		tempG = node->g;
	}
	return (float)(1.0 + ALPHA * (tempG - 1.0));
}

float AStar::calculateH(AStarNode * node)
{
	return (float)(MIN_COST*(abs((float)node->x - (float)goalNode->x) + abs((float)node->y - (float)goalNode->y)));
}

void AStar::showList(std::vector<AStarNode*> nodeList)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		std::cout << nodeList[i]->x << " " << nodeList[i]->y << std::endl;
		std::cout << "F : " << nodeList[i]->f << " G : " << nodeList[i]->g << " H : " << nodeList[i]->h << std::endl;
	}
}