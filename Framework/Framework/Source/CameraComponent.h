#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "Component.h"
#include "Camera.h"

class CameraComponent : public Component
{
public:
	CameraComponent();
	CameraComponent(Camera*);
	virtual ~CameraComponent();

	// Generic Functions
	void CreateComponent(luabridge::LuaRef & tableInfo, std::string name);
	void Update(double dt);
	void LookAround(double dt);

	// Setters and Getters
	void setCamera(Camera * camera);
	Camera * getCamera();

	void setCameraDirection(Vector3 direction);
	Vector3 getCameraDirection();

	void setCameraOffset(Vector3 offset);
	Vector3 getCameraOffset();
private:
	Camera * m_Camera;
	Vector3 m_v3CamDir;
	Vector3 m_v3CamOffset;
};

#endif