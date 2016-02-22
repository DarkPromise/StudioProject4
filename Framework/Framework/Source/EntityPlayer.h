#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H

#include "Entity.h"

#define MOVE_SPEED 200.f
#define ROTATION_SPEED 3.f
#define GRAVITY -30.f
#define JUMP_SPEED 10.f

// Lua
#include "LuaReader.h"

class EntityPlayer : public Entity
{
public:
	EntityPlayer();
	virtual ~EntityPlayer();

	virtual void Update(double dt);
	void Jump();
private:
	bool m_bOnGround;
};

#endif