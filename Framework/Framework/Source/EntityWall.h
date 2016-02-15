#ifndef ENTITY_WALL_H
#define ENTITY_WALL_H

#include "Entity.h"

class EntityWall : public Entity
{
public:
	EntityWall();
	virtual ~EntityWall();

	virtual void Update(double dt);
private:

};

#endif