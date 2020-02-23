#pragma once

#include "Player.h"

class Shop
{
private:
	int priceOfCar[3];

	Player User;

public:
	Shop();
	~Shop();

	void buyCar(int whichCar);

	void sellCar(int whichCar);


};