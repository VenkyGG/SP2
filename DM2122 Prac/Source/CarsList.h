#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem> 
#include "CCar.h"
class CarsList
{
private:
	int numcars;
	CCar* Start;
	CCar* CurrentCar;
public:
	CarsList();
	~CarsList();
	int GetnumberofCars();
	CCar* GetStart();
	void SetStart(CCar * x);
	CCar* GetCar(int index);
	CCar* GetCurrentCar();
	void SetCurrentCar(CCar* car);
};

