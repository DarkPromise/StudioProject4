#ifndef ENTITY_WORLD_OBJECT_H
#define ENTITY_WORLD_OBJECT_H

#include "Entity.h"

class EntityWorldObject : public Entity
{
public:
	EntityWorldObject();
	virtual ~EntityWorldObject();

	virtual void Update(double dt);
private:

};

#endif