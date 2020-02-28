#include "ThirdPersonCam.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

ThirdPersonCamera::ThirdPersonCamera()
{
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	mouseenabledHorizontal = true;
	mouseenabledVertical = true;
	sensitivity = 0.05f;

	RotationEnabled = true;
	useWASD = false;
	disableReset = false;
}

void ThirdPersonCamera::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	Vector3 up2 = Vector3(0, 1, 0);

	if (mouseenabledHorizontal)
	{
		double xpos = Application::getmouseXpos();
		float yaw = sensitivity * -xpos;
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position -= offset;
		position = rotation * position;
		position += offset;
		up = rotation * up;
		Rotationfloat += yaw;
		Application::mouseupdate();
	}
	if (mouseenabledVertical)
	{
		double ypos = Application::getmouseYpos();
		float pitch = sensitivity * -ypos;
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position -= offset;
		position = rotation * position;
		position += offset;
		Rotationfloat2 += pitch;
		Application::mouseupdate();
	}
	if (RotationEnabled)
	{
		
		if (Application::IsKeyPressed(VK_LEFT))
		{
			float yaw = (float)(-CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position -= offset;
			position = rotation * position;
			position += offset;
			up = rotation * up;
		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			float yaw = (float)(CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position -= offset;
			position = rotation * position;
			position += offset;
			up = rotation * up;
		}
		if (Application::IsKeyPressed(VK_UP))
		{
			float pitch = (float)(-CAMERA_SPEED * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position -= offset;
			position = rotation * position;
			position += offset;
		}
		if (Application::IsKeyPressed(VK_DOWN))
		{
			float pitch = (float)(CAMERA_SPEED * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position -= offset;
			position = rotation * position;
			position += offset;
		}
	}

	if (useWASD)
	{
		if (Application::IsKeyPressed('A'))
		{
			float yaw = (float)(-CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position -= offset;
			position = rotation * position;
			position += offset;
			up = rotation * up;
		}
		if (Application::IsKeyPressed('D'))
		{
			float yaw = (float)(CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			position -= offset;
			position = rotation * position;
			position += offset;
			up = rotation * up;
		}
		if (Application::IsKeyPressed('W'))
		{
			float pitch = (float)(-CAMERA_SPEED * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position -= offset;
			position = rotation * position;
			position += offset;
		}
		if (Application::IsKeyPressed('S'))
		{
			float pitch = (float)(CAMERA_SPEED * dt);
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			position -= offset;
			position = rotation * position;
			position += offset;
		}
	}


	if (Application::IsKeyPressed('N'))
	{
		Vector3 direction = target - position;
		if (direction.Length() > 5)
		{
			Vector3 view = (target - position).Normalized();
			position += view * (float)(10.f * dt);
		}
	}
	if (Application::IsKeyPressed('M'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(10.f * dt);
	}

	if (disableReset == false)
	{
		if (Application::IsKeyPressed('R'))
		{
			Reset();
		}
	}
}

void ThirdPersonCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}