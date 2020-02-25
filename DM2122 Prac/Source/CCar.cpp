#include "CCar.h"
#include <iostream>
using namespace std;


CCar::CCar()
{
}

void CCar::setmaxSpeed(float maxspeed)
{
	maxSpeed = maxspeed;
}

float CCar::getmaxSpeed()
{
	return maxSpeed;
}

void CCar::setcurrentSpeed(float speed)
{
	currentSpeed = speed;
}

float CCar::getcurrentSpeed()
{
	return currentSpeed;
}

float CCar::GetPrice()
{
	return price;
}

void CCar::SetPrice(float Price)
{
	price = Price;
}

CCar* CCar::GetNext()
{
	return Next;
}

CCar* CCar::Getprevious()
{
	return Previous;
}

void CCar::SetNext(CCar* x)
{
	Next = x;
}

void CCar::SetPrevious(CCar* x)
{
	Previous = x;
}



void CCar::Spin()
{
	if (IsSpinning)
	{
		SetRotation(0,Vector3(0,GetRotation()[0].y + 1,0));
	}
}

bool CCar::GetIsSpinning()
{
	return IsSpinning;
}

void CCar::SetIsSpinning(bool Spin)
{
	IsSpinning = Spin;
}

bool CCar::GetOwned()
{
	return owned;
}

void CCar::SetOwned(bool x)
{
	owned = x;
}



CCar::~CCar()
{
}
