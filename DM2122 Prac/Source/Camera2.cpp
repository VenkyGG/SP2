#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"
Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	view = (target - position).Normalized();
	right = view.Cross(up).Normalized();
	this->up = defaultUp = right.Cross(view).Normalized();
	playerheight = pos.y;
	xbounds = 17;//x boundary
	zbounds = 25;//z boundary
	mouseenabled = true;
	sensitivity = 0.05f;
}
void Camera2::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	view = (target - position).Normalized();
	right = view.Cross(up);
	Vector3 up2 = Vector3(0, 1, 0);
	right.y = 0;

	if (mouseenabled)
	{
		double xpos = Application::getmouseXpos();
		double ypos = Application::getmouseYpos();
		Mtx44 rotation;
		up = right.Cross(view).Normalized();
		float yaw = sensitivity * -xpos;
		rotation.SetToRotation(yaw, up2.x, up2.y, up2.z);
		view = rotation * view;
		target = position + view;
		Rotationfloat2 += yaw;

		float pitch = sensitivity * -ypos;
		rotation.SetToRotation(pitch, right.x, 0, right.z);
		view = rotation * view;
		up = rotation * up;
		target = position + view;
		Rotationfloat += pitch;

		Application::mouseupdate();
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		up = right.Cross(view).Normalized();
		float yaw = (float)(CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, up2.x, up2.y, up2.z);
		view = rotation * view;
		target = position + view;
		Rotationfloat2 += yaw;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		up = right.Cross(view).Normalized();
		float yaw = (float)(-CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, up2.x, up2.y, up2.z);
		view = rotation * view;
		target = position + view;
		Rotationfloat2 += yaw;

	}
	//if(Application::IsKeyPressed(VK_SPACE))
	//{
	//	position = position + Vector3(0, 1, 0);
	//	target = position + view;
	//}
	if (Application::IsKeyPressed(VK_UP))
	{
		if (Rotationfloat > -60)
		{
			float pitch = (float)(-CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, 0, right.z);
			view = rotation * view;
			up = rotation * up;
			target = position + view;
			Rotationfloat += pitch;
		}
	}
	//if(Application::IsKeyPressed(VK_CONTROL))
	//{
	//	position = position - Vector3(0, 1, 0);
	//	target = position + view;
	//}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (Rotationfloat < 60)
		{
			float pitch = (float)(CAMERA_SPEED * dt);
			up = right.Cross(view).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, 0, right.z);
			view = rotation * view;
			up = rotation * up;
			target = position + view;
			Rotationfloat += pitch;
		}
	}

	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}


}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

//
//if (position.z <= zbounds && position.z >= -zbounds && (position.x >= xbounds + 1 || position.x <= -xbounds - 1))
//{
//	if (position.z > zbounds - 1)
//	{
//		position.z = (zbounds + 0.1f);
//	}
//	if (position.z < -zbounds + 1)
//	{
//		position.z = (-zbounds - 0.6f);
//	}
//}
//if (position.x >= xbounds && position.z <= zbounds && position.z >= -zbounds)
//{
//	position.x = (xbounds - 0.1f);
//}
//if (position.x <= -xbounds && position.z <= zbounds && position.z >= -zbounds)
//{
//	position.x = -(xbounds - 0.1f);
//}