#include "CollisionComponent.h"
#include "Entity.h"

CollisionComponent::CollisionComponent()
: m_AABB(nullptr)
, m_AABBMesh(nullptr)
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

void CollisionComponent::Update(double dt)
{
	auto infoC = this->getParent()->getComponent<InformationComponent>();
	if (infoC)
	{

	}
}

int CollisionComponent::collidesWith(CollisionComponent * collisionInfo, int & score)
{
	if (this->getParent() == collisionInfo->getParent())
	{
		return -1;
	}

	/*if (this->getEntityType() == Entity::ENTITY_BULLET)
	{
		EntityProjectile * test = dynamic_cast<EntityProjectile*>(this);
		if (test->getStatus() == false)
		{
			return -1;
		}
	}
	else if (entity->getEntityType() == Entity::ENTITY_BULLET)
	{
		EntityProjectile * test = dynamic_cast<EntityProjectile*>(entity);
		if (test->getStatus() == false)
		{
			return -1;
		}
	}*/

	BoundingBox * thisBox = new BoundingBox();
	BoundingBox * thatBox = new BoundingBox();
	thisBox->setMaxMin(this->getAABB()->Max + this->getParent()->getComponent<InformationComponent>()->getPosition(), this->getAABB()->Min + this->getParent()->getComponent<InformationComponent>()->getPosition());
	thatBox->setMaxMin(collisionInfo->getAABB()->Max + collisionInfo->getParent()->getComponent<InformationComponent>()->getPosition(), collisionInfo->getAABB()->Min + collisionInfo->getParent()->getComponent<InformationComponent>()->getPosition());

	// Basic Collision Check
	if ((thisBox->Min.x < thatBox->Max.x) && (thisBox->Max.x > thatBox->Min.x) &&
		(thisBox->Min.y < thatBox->Max.y) && (thisBox->Max.y > thatBox->Min.y) &&
		(thisBox->Min.z < thatBox->Max.z) && (thisBox->Max.z > thatBox->Min.z))
	{
		Vector3 randomPosition = Vector3(Math::RandFloatMinMax(-1000.f, 1000.f), Math::RandFloatMinMax(0.f,2000.f) , Math::RandFloatMinMax(-1000.f, 1000.f));
		if ((this->getParent()->getComponent<InformationComponent>()->getType() == InformationComponent::ENTITY_TYPE::TYPE_NPC) && collisionInfo->getParent()->getComponent<InformationComponent>()->getType() == InformationComponent::ENTITY_TYPE::TYPE_PLAYER)
		{
			this->getParent()->getComponent<InformationComponent>()->setPosition(randomPosition);
			score++;

			if ((this->getParent()->getComponent<AIComponent>()->getType() == AIComponent::AI_ENEMY))
			{
				auto * healthComponent = collisionInfo->getParent()->getComponent<HealthComponent>();
				if (healthComponent)
				{
					collisionInfo->getParent()->getComponent<HealthComponent>()->setHealth(collisionInfo->getParent()->getComponent<HealthComponent>()->getHealth() - 1);
				}
			}
			else if ((this->getParent()->getComponent<AIComponent>()->getType() == AIComponent::AI_SUPPORT))
			{
				auto * healthComponent = collisionInfo->getParent()->getComponent<HealthComponent>();
				if (healthComponent)
				{
					if (healthComponent->getHealth() < healthComponent->getMaxHealth())
					{
						collisionInfo->getParent()->getComponent<HealthComponent>()->setHealth(collisionInfo->getParent()->getComponent<HealthComponent>()->getHealth() + 1);
					}
				}
			}
		}
		else if (collisionInfo->getParent()->getComponent<InformationComponent>()->getType() == InformationComponent::ENTITY_TYPE::TYPE_NPC)
		{

			collisionInfo->getParent()->getComponent<InformationComponent>()->setPosition(randomPosition);
			score++;
			if ((collisionInfo->getParent()->getComponent<AIComponent>()->getType() == AIComponent::AI_ENEMY))
			{
				auto * healthComponent = this->getParent()->getComponent<HealthComponent>();
				if (healthComponent)
				{
					this->getParent()->getComponent<HealthComponent>()->setHealth(this->getParent()->getComponent<HealthComponent>()->getHealth() - 1);
				}
			}
			else if ((collisionInfo->getParent()->getComponent<AIComponent>()->getType() == AIComponent::AI_SUPPORT))
			{
				auto * healthComponent = this->getParent()->getComponent<HealthComponent>();
				if (healthComponent)
				{
					if (healthComponent->getHealth() < healthComponent->getMaxHealth())
					{
						this->getParent()->getComponent<HealthComponent>()->setHealth(this->getParent()->getComponent<HealthComponent>()->getHealth() + 1);
					}
				}
			}
		}
		//std::cout << "Collided" << std::endl;
		delete thisBox;
		delete thatBox;
		return 1;
	}
	delete thisBox;
	delete thatBox;
	return -1;
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

