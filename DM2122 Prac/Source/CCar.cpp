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

string CCar::getfileLocation()
{
	return fileLocation;
}

void CCar::setfileLocation(string x)
{
	fileLocation = x;
}

string CCar::getTextureLocation()
{
	return textureLocation;
}

void CCar::setTextureLocation(string x)
{
	textureLocation = x;
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

Mesh* CCar::GetMesh()
{
	return mesh;
}

void CCar::SetMesh(string filelocation)
{
	mesh = MeshBuilder::GenerateOBJ("CAR", filelocation);
}

bool CCar::GetIsSpinning()
{
	return IsSpinning;
}

void CCar::SetIsSpinning(bool x)
{
	IsSpinning = x;
}


CCar::~CCar()
{
}
