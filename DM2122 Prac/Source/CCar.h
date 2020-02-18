#pragma once
using namespace std;
#include <string>

class CCar
{
	float maxSpeed;
	float currentSpeed;
	float xLocation;
	float zLocation;
	string fileLocation;
	string textureLocation;
	bool IsSpinning;
	float price;

	CCar* Next;
	CCar* Previous;

public:
	void setmaxSpeed(float maxspeed);
	float getmaxSpeed();
	void setcurrentSpeed(float speed);
	float getcurrentSpeed();
	string getfileLocation();
	void setfileLocation(string x);
	string getTextureLocation();
	void setTextureLocation(string x);
	void setxLocation(float xpos);
	float getxLocation();
	void setzLocation(float zpos);
	float getzLocation();

	CCar* GetNext();
	CCar* Getprevious();
	void SetNext(CCar* x);
	void SetPrevious(CCar* x);

	CCar();
	~CCar();
};

