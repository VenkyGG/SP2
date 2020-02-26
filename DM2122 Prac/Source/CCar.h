#pragma once

#include "Object.h"

class CCar:public Object
{
private:
	float maxSpeed;
	float currentSpeed;

	bool IsSpinning;
	int price;
	bool owned;
	
	CCar* Next;
	CCar* Previous;

public:
	void setmaxSpeed(float maxspeed);
	float getmaxSpeed();
	void setcurrentSpeed(float speed);
	float getcurrentSpeed();
	
	int GetPrice();
	void SetPrice(int Price);
	CCar* GetNext();
	CCar* Getprevious();
	void SetNext(CCar* x);
	void SetPrevious(CCar* x);
	
	void Spin();
	bool GetIsSpinning();
	void SetIsSpinning(bool Spin);

	bool GetOwned();
	void SetOwned(bool x);

	CCar();
	~CCar();
};

