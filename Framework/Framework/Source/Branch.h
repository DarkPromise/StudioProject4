#ifndef BRANCH_H
#define BRANCH_H

#define MAX_SUBPARTITIONS 8

#include "Vector3.h"
#include "MeshBuilder.h"
#include <iostream>

class Entity;
class View;

class Branch
{
public:
	Branch(Branch * parent, int level, Vector3 center, Vector3 size);
	~Branch();

	void createSubPartitions(int levels);
	void addObject(Entity * entity);
	void removeObject(int id);
	void checkCollisions(int & noTests, int & noCollisions, int & score);
	void checkBorderCollisions(Branch * branch,int & noTests, int & noCollisions, int & score);
	
	void UpdateAIComponents(double dt, Entity * thePlayer);
	void UpdateLevelOfDetails(Vector3 camPos);

	void PrintInfo();
	void PrintListInfo();
	void Render(View * theView);

	bool canContain(Entity * entity);


	Mesh * getMesh();
	void setMesh(Mesh * mesh);
	Branch * getRoot();
	Vector3 getCenter();
	int getLevel();

	bool hasChild() { return (child[0] != NULL); }
private:
	Branch * parent;
	Branch * child[MAX_SUBPARTITIONS];

	Vector3 m_cBranchCenter;
	Vector3 m_cMin;
	Vector3 m_cMax;

	int m_iLevel;
	Color m_color;
	Mesh * m_cMesh;
	
	std::vector<Entity*> m_entityList;
};

#endif