#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"
#include "MeshBuilder.h"
#include "AxisAlignedBoundingBox.h"

class CollisionComponent : public Component
{
public:
	CollisionComponent();
	virtual ~CollisionComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name);
	void Update(double dt);

	int collidesWith(CollisionComponent * collisionInfo, int & score);

	void setMinMax(Vector3 min, Vector3 max);
	BoundingBox * getAABB();

	void setMesh(Mesh * newMesh);
	Mesh * getMesh();
private:
	Mesh * m_AABBMesh;
	BoundingBox * m_AABB;
};

#endif