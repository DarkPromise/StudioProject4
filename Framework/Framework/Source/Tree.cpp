#include "Tree.h"

Tree::Tree()
{
	this->root = NULL;
}

Tree::~Tree()
{
	delete root;
}

void Tree::createTree(int levels, Vector3 center, Vector3 size)
{
	root = new Branch(NULL, 0, center, size);
	root->createSubPartitions(levels);
}

void Tree::checkCollisions(int & noTests, int & noCollisions, int & score)
{
	noTests = 0;
	noCollisions = 0;
	root->checkCollisions(noTests, noCollisions, score);
}

void Tree::addObject(Entity * entity)
{
	root->addObject(entity);
}

void Tree::PrintInfo()
{
	if (this->root != NULL)
	{
		root->PrintInfo();
	}
}

void Tree::PrintListInfo()
{
	if (this->root != NULL)
	{
		root->PrintListInfo();
	}
}

void Tree::Render(View * theView)
{
	root->Render(theView);
}

void Tree::UpdateAIComponents(double dt, Entity * thePlayer)
{
	root->UpdateAIComponents(dt, thePlayer);
}

void Tree::UpdateLevelOfDetails(Vector3 camPos)
{
	root->UpdateLevelOfDetails(camPos);
}