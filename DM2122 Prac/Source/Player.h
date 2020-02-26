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
	int startingBalance = 50000; // Change this to alter the starting balance.

	int money;

	static Player* instances;

	std::string File = "player.txt"; // Change this if you want to have a different txt file for player.

	float time;

	

	std::string storeLines[10];

	std::string line1; // Do not touch.
	std::string line2; // Do not touch.
	std::string line3; // Do not touch.
	std::string line4; // Do not touch.
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

	void PrintOwnedCars();

	void rewriteFile();
	bool carsOwned[5];
	static Player* instance()
	{
		if (!instances)
		{
			instances = new Player;
		}
		return instances;
	}

	CarsList cars;
};