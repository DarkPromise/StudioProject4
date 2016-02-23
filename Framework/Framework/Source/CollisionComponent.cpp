#include "CollisionComponent.h"
#include "Entity.h"

CollisionComponent::CollisionComponent()
: m_AABB(NULL)
, m_AABBMesh(NULL)
{

}

CollisionComponent::~CollisionComponent()
{
	if (m_AABB)
	{
		delete m_AABB;
	}
	if (m_AABBMesh)
	{
		delete m_AABBMesh;
	}
}

void CollisionComponent::CreateComponent(luabridge::LuaRef& tableInfo, std::string name)
{

}

void CollisionComponent::Update(double dt)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{

	}
}

int CollisionComponent::collidesWith(CollisionComponent * collisionInfo, int & score)
{
	return 0;
}

void CollisionComponent::setMinMax(Vector3 min, Vector3 max)
{
	this->m_AABB = new BoundingBox();

	this->m_AABB->Max.x = max.x;
	this->m_AABB->Max.y = max.y;
	this->m_AABB->Max.z = max.z;

	this->m_AABB->Min.x = min.x;
	this->m_AABB->Min.y = min.y;
	this->m_AABB->Min.z = min.z;
}

BoundingBox * CollisionComponent::getAABB()
{
	return this->m_AABB;
}


void CollisionComponent::setMesh(Mesh * newMesh)
{
	this->m_AABBMesh = newMesh;
}

Mesh * CollisionComponent::getMesh()
{
	return this->m_AABBMesh;
}

