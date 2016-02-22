#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include "Component.h"

#include "MeshBuilder.h"

class GraphicsComponent : public Component
{
public:
	enum RESOLUTION
	{
		RESOLUTION_LOW = 0,
		RESOLUTION_MED,
		RESOLUTION_HIGH,
	};

	GraphicsComponent();
	virtual ~GraphicsComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo);
	void Update(double dt);

	// Mesh Creation
	void addMesh(Mesh * mesh);
	Mesh * getMesh(int index = 0);

	void setResLevel(int i);
	int getResLevel();

private:
	std::vector<Mesh*> m_meshList;
	int m_iResLevel;
};

#endif