#pragma once

#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <cstdint>

using namespace std;

class Player
{
private:
	int startingBalance = 50000; // Change this to alter the starting balance.

	int money;

	std::string File = "player.txt"; // Change this if you want to have a different txt file for player.

	float time;

	std::string carsOwned[5];

	std::string line1; // Do not touch.
	std::string line2; // Do not touch.
	std::string line3; // Do not touch.
	std::string line4; // Do not touch.

public:
	Player();
	~Player();

	bool hasFile();

	void createFile();

	void initMoney();
	void initOwnedCars();

	void removeMoney(int amountToBeRemoved);
	void addMoney(int amountToBeAdded);

	void removeCar(int whichCar);
	void addCar(int whichCar);

	int getMoney();
	bool hasCar(int whichCar);

	void PrintOwnedCars();

	void rewriteFile();
};