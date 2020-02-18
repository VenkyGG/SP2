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
	bool IsSpinning;
	float price;

public:
	void setmaxSpeed(float maxspeed);
	float getmaxSpeed();
	void setcurrentSpeed(float speed);
	float getcurrentSpeed();
	string getfileLocation();
	void setxLocation(float xpos);
	float getxLocation();
	void setzLocation(float zpos);
	float getzLocation();

	CCar();
	~CCar();
};

