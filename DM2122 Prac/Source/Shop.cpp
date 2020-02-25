#include "Shop.h"

Shop::Shop()
{
	priceOfCar[0] = 20000;
	priceOfCar[1] = 40000;
	priceOfCar[2] = 10000;

}

Shop::~Shop()
{
}

//void Shop::buyCar(int whichCar)
//{
//	Player User = Player::instance();
//	if (User.hasCar(whichCar) == false)
//	{
//		bool tmpCheck;
//		tmpCheck = User.removeMoney(priceOfCar[whichCar]);
//
//		if (tmpCheck == true)
//		{
//			User.addCar(whichCar);
//
//			std::cout << "You have bought the Car! " << whichCar << std::endl;
//		}
//	}
//	else
//	{
//		std::cout << "You already have the car!" << std::endl;
//	}
//}
//
//void Shop::sellCar(int whichCar)
//{
//	if (User.hasCar(whichCar) == true)
//	{
//		User.addMoney(priceOfCar[whichCar]);
//
//		User.removeCar(whichCar);
//		
//		std::cout << "You have successfull sold your Car!" << std::endl;
//	}
//	else
//	{
//		std::cout << "You do not own this car!" << std::endl;
//	}
//
//}
