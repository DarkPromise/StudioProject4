#ifndef ENTITY_HERO_H
#define ENTITY_HERO_H

#include "Entity.h"

class EntityHero : public Entity
{
public:
	EntityHero();
	virtual ~EntityHero();

	virtual void Update(double dt);
private:
};

#endif