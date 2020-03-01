#pragma once

#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <cstdint>
#include "CarsList.h"
#include <irrKlang.h>
#pragma comment(lib,"irrKlang.lib")

using namespace std;

class Player
{
private:
	int startingBalance;

	int money;

	static Player* instances;

	std::string File;

	std::string storeLines[5];

	Player();
	~Player();

public:
	void CheckAddSound(bool);
	bool hasFile();

	void createFile();

	void initMoney();
	void initOwnedCars();

	bool removeMoney(int amountToBeRemoved);
	void addMoney(int amountToBeAdded);

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