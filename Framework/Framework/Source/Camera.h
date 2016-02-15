#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "MatrixStack.h"
#include "Mtx44.h"

#include "InputHandler.h"

class Camera
{
public:
	enum CAMERA_MODE
	{
		CM_FIRST_PERSON,
		CM_THIRD_PERSON_FIXED_OFFSET,
		CM_THIRD_PERSON_FOLLOW_ENTITY,
		CM_FREECAM
	};

	Camera();
	virtual ~Camera();

	virtual void Update(double dt,InputHandler * theInputHandler);

	void setCameraPos(Vector3 pos);
	Vector3 getCameraPos();

	void setCameraTarget(Vector3 target);
	Vector3 getCameraTarget();

	void setCameraUp(Vector3 up);
	Vector3 getCameraUp();

	void setCameraPitch(float pitch);
	float getCameraPitch();

	void setCameraYaw(float yaw);
	float getCameraYaw();

	void setCameraRoll(float roll);
	float getCameraRoll();

	void setCameraMode(CAMERA_MODE mode);
	CAMERA_MODE getCameraMode();
public:
	MS getViewMatrix();
	void limitValues();
	void lookAround();
private:
	Vector3 m_v3CamPos;
	Vector3 m_v3CamTarget;
	Vector3 m_v3CamUp;

	float m_pitch;
	float m_yaw;
	float m_roll;

	CAMERA_MODE m_cameraMode;
};

#endif