#ifndef THIRDPERSONCAMERA_H
#define THIRDPERSONCAMERA_H

#include "Camera.h"
#include <Mtx44.h>
class ThirdPersonCamera : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	float Rotationfloat;
	float Rotationfloat2;
	
	Vector3 offset;
	Vector3 view;
	Vector3 right;
	bool mouseenabled;
	float sensitivity;
	bool RotationEnabled = true;
	bool useWASD = false;

	ThirdPersonCamera();
	~ThirdPersonCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif