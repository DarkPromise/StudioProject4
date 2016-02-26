#ifndef ENTITY_H
#define ENTITY_H

#pragma warning(disable:4018)

class Component;

// C++ Includes
#include <iostream>
#include <string>
#include <vector>

// Component Includes
#include "InformationComponent.h"
#include "GraphicsComponent.h"
#include "AIComponent.h"
#include "HealthComponent.h"
#include "PhysicsComponent.h"
#include "WaypointComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "ControllerComponent.h"
#include "GameplayComponent.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void Update(double dt);

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

public:
	// Entity Creation Functions

private:
	std::vector<Component*> m_componentList;
};

#endif