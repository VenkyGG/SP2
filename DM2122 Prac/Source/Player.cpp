#include "Player.h" 

Player::Player()
{
	startingBalance = 100000;

	File = "player.txt";

	bool check = false;

	check = hasFile();
	
	if (check == false)
	{
		std::cout << "Generating Default file!" << std::endl;

		createFile();

		initMoney();

		initOwnedCars();
	}
	else
	{
		initMoney();

		initOwnedCars();
	}
}

Player::~Player()
{
	rewriteFile();
	if (instances!=NULL)
	{
		for (int i = cars.GetnumberofCars()-1; i > -1; i--)
		{
			delete cars.GetCar(i);
			cout << "HI" << endl;
		}
		delete instances;
		instances = NULL;
	}
}

bool Player::hasFile()
{
	ifstream runTime(File);

	if (runTime.fail())
	{
		std::cout << "Unable to open file! "/* << std::endl*/;

		return false;
	}
	else
	{
		return true;
	}
}

void Player::createFile()
{
	ofstream runTime(File);

	runTime << "50c464d33fd802fc1bd247499d037340\n";
	runTime << "Player 1\n";
	runTime << "fb1217a508f8bf839aba8b87afaef694\n";
	runTime << to_string(startingBalance) + "\n";
	runTime << "b799f7f2affa11d74804bd574f04de8a\n";
	runTime << "UNOWNED\n";
	runTime << "UNOWNED\n";
	runTime << "UNOWNED\n";
	runTime << "UNOWNED\n";
	runTime << "UNOWNED\n";
}

void Player::initMoney()
{
	ifstream runTime(File);

	string line;

	int lineNumber = 1;

	if (runTime.is_open())
	{
		while (getline(runTime, line))
		{
			if (lineNumber == 4)
			{
				money = stoi(line);
			}

			lineNumber++;

			if (lineNumber == 11) // Break out of look such that doesnt check other lines.
			{
				break;
			}
		}
	}
}

void Player::initOwnedCars()
{
	ifstream runTime(File);

	string line;

	int lineNumber = 1;

	if (runTime.is_open())
	{
		while (getline(runTime, line))
		{
			if (lineNumber == 6)
			{
				if (line == "OWNED")
				{
					carsOwned[0] = true;
					cars.GetCar(0)->SetOwned(true);
				}
				else
				{
					carsOwned[0] = false;
					cars.GetCar(0)->SetOwned(false);
				}
			}

			if (lineNumber == 7)
			{
				if (line == "OWNED")
				{
					carsOwned[1] = true;
					cars.GetCar(1)->SetOwned(true);
				}
				else
				{
					carsOwned[1] = false;
					cars.GetCar(1)->SetOwned(false);

				}
			}

			if (lineNumber == 8)
			{
				if (line == "OWNED")
				{
					carsOwned[2] = true;
					cars.GetCar(2)->SetOwned(true);
				}
				else
				{
					carsOwned[2] = false;
					cars.GetCar(2)->SetOwned(false);
				}
			}

			if (lineNumber == 9)
			{
				if (line == "OWNED")
				{
					carsOwned[3] = true;
					cars.GetCar(3)->SetOwned(true);
				}
				else
				{
					carsOwned[3] = false;
					cars.GetCar(3)->SetOwned(false);
				}
			}

			if (lineNumber == 10)
			{
				if (line == "OWNED")
				{
					carsOwned[4] = true;
					cars.GetCar(4)->SetOwned(true);
				}
				else
				{
					carsOwned[4] = false;
					cars.GetCar(4)->SetOwned(false);
				}
			}

			lineNumber++;

			if (lineNumber == 11) // Break out of look such that doesnt check other lines.
			{
				break;
			}
		}
	}
	

}

bool Player::removeMoney(int amountToBeRemoved) // Function to remove money.
{
	int tempBalance = getMoney() - amountToBeRemoved;

	bool isItNegativeBalance = false;

	if (tempBalance < 0) // Checks if Player's Balance is negative.
	{
		isItNegativeBalance = true;
	}

	if (isItNegativeBalance == true) // If Player's Balance is negative, then money wont be removed.
	{
		//std::cout << "Not enough money!" << std::endl;

		return false;
	}
	else
	{
		money = getMoney() - amountToBeRemoved;
		rewriteFile();
		return true;
	}

	
}

void Player::addMoney(int amountToBeAdded) // Function to add money.
{
	money = getMoney() + amountToBeAdded;

	rewriteFile();
}

void Player::removeCar(int whichCar)
{
	carsOwned[whichCar] = "UNOWNED";
	rewriteFile();
}

void Player::addCar(int whichCar)
{
	carsOwned[whichCar] = "OWNED";
	rewriteFile();
}

int Player::getMoney()
{
	return money;
}

void Player::rewriteFile()
{
	ifstream readTest(File);
	string line;

	int lineNumber = 1;

	if (readTest.is_open())
	{
		while (getline(readTest, line))
		{
			storeLines[lineNumber - 1] = line;

			lineNumber++;

			if (lineNumber == 6) // Break out of loop such that doesnt check other lines.
			{
				break;
			}
		}
	}

	readTest.close();

	if (remove("player.txt") != 0)
	{
		perror("Error deleting file");
	}
	

	ofstream write(File);

	for (int counter = 1; counter <= 10; ++counter)
	{
		if (counter >= 1 && counter <= 5  && counter!=4)
		{
			write << storeLines[counter - 1] + "\n";
		}
		if (counter == 4)
		{
			write << to_string(getMoney()) + "\n";
		}
		else if (counter == 6)
		{
			if (cars.GetCar(0)->GetOwned())
			{
				write << "OWNED\n";
			}
			else
			{
				write << "UNOWNED\n";
			}
		}
		else if (counter == 7)
		{
			if (cars.GetCar(1)->GetOwned())
			{
				write << "OWNED\n";
			}
			else
			{
				write << "UNOWNED\n";
			}
		}
		else if (counter == 8)
		{
			if (cars.GetCar(2)->GetOwned())
			{
				write << "OWNED\n";
			}
			else
			{
				write << "UNOWNED\n";
			}
		}
		else if (counter == 9)
		{
			if (cars.GetCar(3)->GetOwned())
			{
				write << "OWNED\n";
			}
			else
			{
				write << "UNOWNED\n";
			}
		}
		else if (counter == 10)
		{
			if (cars.GetCar(4)->GetOwned())
			{
				write << "OWNED\n";
			}
			else
			{
				write << "UNOWNED\n";
			}
		}
		
	}
}