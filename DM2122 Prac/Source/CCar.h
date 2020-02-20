#pragma once

#include "Object.h"
using namespace std;
class CCar:public Object
{
private:
	float maxSpeed;
	float currentSpeed;

	bool IsSpinning;
	float price;

	
	CCar* Next;
	CCar* Previous;

public:
	void setmaxSpeed(float maxspeed);
	float getmaxSpeed();
	void setcurrentSpeed(float speed);
	float getcurrentSpeed();
	

	CCar* GetNext();
	CCar* Getprevious();
	void SetNext(CCar* x);
	void SetPrevious(CCar* x);
	
	void Spin();
	bool GetIsSpinning();
	void SetIsSpinning(bool Spin);


	CCar();
	~CCar();
};

