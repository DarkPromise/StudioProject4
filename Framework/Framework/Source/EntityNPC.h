#ifndef ENTITY_NPC_H
#define ENTITY_NPC_H

#include "Entity.h"

#define MOVE_SPEED 200.f
#define GRAVITY -30.f
#define JUMP_SPEED 10.f

class EntityNPC : public Entity
{
public:
	EntityNPC();
	virtual ~EntityNPC();

	virtual void Update(double dt);

	void Test();

	void Jump();
private:
	bool m_bOnGround;
};

#endif