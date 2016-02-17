#ifndef ENTITY_H
#define ENTITY_H

class Component;

#include <iostream>
#include <string>
#include <vector>

#include "InformationComponent.h"
#include "GraphicsComponent.h"
#include "AIComponent.h"
#include "HealthComponent.h"
#include "PhysicsComponent.h"
#include "WaypointComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "ControllerComponent.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void Update(double dt) = 0;
	void addComponent(Component* component);

	template<typename T> T* getComponent()
	{
		T * result;
		for (int i = 0; i < m_componentList.size(); i++)
		{
			if (result = dynamic_cast<T*>(m_componentList[i]))
			{
				return result;
			}
		}
		//std::cout << "Component does not exist" << std::endl;
		return nullptr;
	}
private:
	std::vector<Component*> m_componentList;
};

#endif