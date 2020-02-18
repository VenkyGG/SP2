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
	return maxSpeed;
}

string CCar::getfileLocation()
{
	return fileLocation;
}

void CCar::setxLocation(float xpos)
{
	xLocation = xpos;
}

float CCar::getxLocation()
{
	return xLocation;
}

void CCar::setzLocation(float zpos)
{
	zLocation = zpos;
}

float CCar::getzLocation()
{
	return zLocation;
}

CCar::~CCar()
{
}
