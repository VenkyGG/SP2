#include "Shop.h"

Shop::Shop()
{
}

Shop::~Shop()
{
}

bool Shop::buyCar(CCar* whichCar)
{
	if (Player::instance()->cars.GetCurrentCar()->GetOwned() == false)
	{
		bool tmpCheck;

		tmpCheck = Player::instance()->removeMoney(Player::instance()->cars.GetCurrentCar()->GetPrice());

		if (tmpCheck == true)
		{
			Player::instance()->cars.GetCurrentCar()->SetOwned(true);

			Player::instance()->rewriteFile();

			//std::cout << "You have bought the Car! " << whichCar << std::endl;

			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		//std::cout << "You already have the car!" << std::endl;

		//Application::state = Application::Driving;

		return true;
	}
}