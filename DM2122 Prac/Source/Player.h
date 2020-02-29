#pragma once

#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <cstdint>
#include "CarsList.h"

using namespace std;

class Player
{
private:
	int startingBalance; // Change this to alter the starting balance.

	int money;

	static Player* instances;

	std::string File; // Change this if you want to have a different txt file for player.

	std::string storeLines[5];

	Player();
	~Player();

public:

	bool hasFile();

	void createFile();

	void initMoney();
	void initOwnedCars();

	bool removeMoney(int amountToBeRemoved);
	void addMoney(int amountToBeAdded);

	void removeCar(int whichCar);
	void addCar(int whichCar);

	int getMoney();

	void rewriteFile();
	
	static Player* instance()
	{
		if (!instances)
		{
			instances = new Player;
		}
		return instances;
	}

	CarsList cars;

	bool carsOwned[5];

	static void shutdown();
};