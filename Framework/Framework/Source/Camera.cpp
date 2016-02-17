#include "Camera.h"

Camera::Camera()
: m_v3CamPos(Vector3(0.f,0.f,100.f))
, m_v3CamTarget(Vector3(0.f,0.f,0.f))
, m_v3CamUp(Vector3(0.f,1.f,0.f))
, m_pitch(0.f)
, m_yaw(0.f)
, m_roll(0.f)
, m_cameraMode(CAMERA_MODE::CM_FREECAM)
{

}

Camera::~Camera()
{

}

MS Camera::getViewMatrix()
{
	MS viewStack;
	viewStack.LookAt(m_v3CamPos.x, m_v3CamPos.y, m_v3CamPos.z, m_v3CamTarget.x, m_v3CamTarget.y, m_v3CamTarget.z, m_v3CamUp.x, m_v3CamUp.y, m_v3CamUp.z);
	return viewStack;
}

void Camera::Update(double dt, InputHandler * theInputHandler)
{
	if (!theInputHandler->isMouseEnabled())
	{
		this->m_yaw += (float)theInputHandler->getDeltaX();
		this->m_pitch += (float)theInputHandler->getDeltaY();
		limitValues();
	}
}

void Camera::setCameraPos(Vector3 pos)
{
	this->m_v3CamPos = pos;
}

Vector3 Camera::getCameraPos()
{
	return this->m_v3CamPos;
}


void Camera::setCameraTarget(Vector3 target)
{
	this->m_v3CamTarget = target;
}

Vector3 Camera::getCameraTarget()
{
	return this->m_v3CamTarget;
}

void Camera::setCameraUp(Vector3 up)
{
	this->m_v3CamUp = up;
}

Vector3 Camera::getCameraUp()
{
	return this->m_v3CamUp;
}

void Camera::setCameraPitch(float pitch)
{
	this->m_pitch = pitch;
}

float Camera::getCameraPitch()
{
	return this->m_pitch;
}

void Camera::setCameraYaw(float yaw)
{
	this->m_yaw = yaw;
}

float Camera::getCameraYaw()
{
	return this->m_yaw;
}

void Camera::setCameraRoll(float roll)
{
	this->m_roll = roll;
}

float Camera::getCameraRoll()
{
	return this->m_roll;
}

void Camera::setCameraMode(CAMERA_MODE mode)
{
	this->m_cameraMode = mode;
}

Camera::CAMERA_MODE Camera::getCameraMode()
{
	return this->m_cameraMode;
}

void Camera::limitValues()
{
	if (m_pitch > 90.f)
	{
		m_pitch = 90.f;
	}
	if (m_pitch < -90.f)
	{
		m_pitch = -90.f;
	}
}

void Camera::lookAround()
{
	//TODO Edit Camera View & Target

	std::cout << "Yaw[" << m_yaw << "] Pitch[" << m_pitch << "]" << std::endl;
}