#pragma once

#include "Player.h"
#include "CarsList.h"
#include "CCar.h"

class Shop
{
private:
	//int priceOfCar[3];

public:
	Shop();
	~Shop();

	void buyCar(CCar* whichCar);

	///void sellCar(int whichCar);
};