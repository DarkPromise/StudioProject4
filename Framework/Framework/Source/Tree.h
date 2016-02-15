#ifndef TREE_H
#define TREE_H

#include "Vector3.h"
#include "Branch.h"
#include "Entity.h"
#include "View.h"

class Tree
{
public:
	Tree();
	~Tree();

	void createTree(int levels, Vector3 center, Vector3 size);
	void checkCollisions(int & noTests,int & noCollisions, int & score);
	void addObject(Entity* entity);

	void UpdateLevelOfDetails(Vector3 camPos);
	void UpdateAIComponents(double dt, Entity * thePlayer);

	void PrintInfo();
	void PrintListInfo();
	void Render(View * theView);

	Branch * root;
};

#endif