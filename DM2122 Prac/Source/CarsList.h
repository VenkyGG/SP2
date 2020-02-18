#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem> 
#include <iostream>
#include "CCar.h"
class CarsList
{
private:
	int numcars;
	CCar* Start;
public:
	CarsList();
	~CarsList();
	int GetnumberofCars();
	CCar* GetStart();
	void SetStart(CCar * x);

};

