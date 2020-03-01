#include "Player.h" 


irrklang::ISoundEngine* engine6 = irrklang::createIrrKlangDevice();
irrklang::ISoundSource* Mon = engine6->addSoundSourceFromFile("Sound/ChaChing.wav");//when money added
irrklang::ISound* msound = engine6->play2D(Mon, true, true, true, false);

Player::Player()
{
	startingBalance = 100000; // Change this value to alter the starting balance of the player

	File = "player.txt"; // Change this value to alter the file name to be saved

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

}

bool Player::hasFile() // This function checks if the file is present in the player's computer.
{
	ifstream runTime(File);

	if (runTime.fail())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Player::createFile() // This function creates the file if the player does not have player.txt
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

void Player::initMoney() // This function initiliazes the player's balance
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

void Player::initOwnedCars() // This function initializes the player's cars that he/she owns
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

bool Player::removeMoney(int amountToBeRemoved) // Function to remove money. Returns a true if money is actually removed so that other functions can check for it. More flexibility
{
	int tempBalance = getMoney() - amountToBeRemoved;

	bool isItNegativeBalance = false;

	if (tempBalance < 0) // Checks if Player's Balance is negative.
	{
		isItNegativeBalance = true;
	}

	if (isItNegativeBalance == true) // If Player's Balance is negative, then money wont be removed.
	{
		return false;
	}
	else
	{
		money = getMoney() - amountToBeRemoved; // This removed player's Money

		rewriteFile();

		return true;
	}
}

void Player::addMoney(int amountToBeAdded) // Function to add money.
{
	money = getMoney() + amountToBeAdded;

	//msound->setIsPaused(false);
	rewriteFile();
}

void Player::CheckAddSound(bool x)
{
	if (x == false)
	{
		msound->setIsPaused(true);
	}
	if (x == true)
	{
		msound->setIsPaused(false);
	}
}

int Player::getMoney()
{
	return money;
}

void Player::rewriteFile() // This function will basically deletes the Player's file and Rewrite it again. Storing information onto player.txt
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

void Player::shutdown()
{
	instances->rewriteFile();
	delete instances;
	instances = NULL;
}