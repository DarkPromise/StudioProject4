#ifndef WAYPOINT_COMPONENT_H
#define WAYPOINT_COMPONENT_H

#include "Component.h"
#include "Vector3.h"

#include <vector>

struct Waypoint
{
	Waypoint()
	: position(Vector3(0.f,0.f,0.f))
	, occupied(false)
	, preoccupied(false)
	{

	}

	Vector3 position;
	bool occupied;
	bool preoccupied;
	Entity * owner;
};

class WaypointComponent : public Component
{
public:
	WaypointComponent();
	virtual ~WaypointComponent();

	void Update(double dt);

	void setWaypointIndex(int index);
	int getWaypointIndex();

	void setTempWaypointIndex(int tempIndex);
	int getTempWaypointIndex();

	void setWaypoints(std::vector<Waypoint*> waypoints);
	void addWaypoint(Waypoint * waypoint);
	std::vector<Waypoint*> getWaypoints();
private:
	int m_iWaypointIndex;
	int m_iTempWaypointIndex;
	std::vector<Waypoint*> m_iWaypoints;
};

#endif