#ifndef WAYPOINT_COMPONENT_H
#define WAYPOINT_COMPONENT_H

#include "Component.h"
#include "Vector3.h"

#include <vector>

#include "GridMap.h"

struct Waypoint
{
	Waypoint()
	: position(Vector3(0.f,0.f,0.f))
	, occupied(false)
	, preoccupied(false)
	, owner(nullptr)
	{

	}

	Vector3 position;
	int indexX;
	int indexY;
	bool occupied;
	bool preoccupied;
	Entity * owner;
};

class WaypointComponent : public Component
{
public:
	WaypointComponent();
	virtual ~WaypointComponent();

	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name, GridMap * gridMap);
	void Update(double dt);

	void setWaypointIndex(int index);
	int getWaypointIndex();

	void setNextWaypointIndex(int nextIndex);
	int getNextWaypointIndex();

	void setWaypoints(std::vector<Waypoint*> waypoints);
	void addWaypoint(Waypoint * waypoint);
	std::vector<Waypoint*> getWaypoints();
private:
	int m_iWaypointIndex;
	int m_iNextWaypointIndex;
	std::vector<Waypoint*> m_iWaypoints;
};

#endif