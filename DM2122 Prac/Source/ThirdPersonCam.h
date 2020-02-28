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
	float Rotationfloat = 0;
	float Rotationfloat2 = 0;
	
	Vector3 offset;
	Vector3 view;
	Vector3 right;
	bool mouseenabledHorizontal;
	bool mouseenabledVertical;
	float sensitivity;

	bool RotationEnabled;
	bool useWASD;
	bool disableReset;

	ThirdPersonCamera();
	~ThirdPersonCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif