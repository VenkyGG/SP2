#include "Shop.h"

irrklang::ISoundEngine* PlSound = irrklang::createIrrKlangDevice();
irrklang::ISoundSource* money = PlSound->addSoundSourceFromFile("Sound/moneyFx.ogg"); // Init folder
irrklang::ISound* moneyx = PlSound->play2D(money, false, true, true, false); // Run Sound

irrklang::ISoundSource* cannotBuy = PlSound->addSoundSourceFromFile("Sound/cannotBuy.ogg"); // Init folder
irrklang::ISound* failBuy = PlSound->play2D(cannotBuy, false, true, true, false); // Run Sound

irrklang::ISoundSource* startCar = PlSound->addSoundSourceFromFile("Sound/carStart.ogg"); // Init folder
irrklang::ISound* car = PlSound->play2D(startCar, false, true, true, false); // Run Sound

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

			PlSound->play2D(money);

			return true;
		}
		else
		{
			PlSound->play2D(cannotBuy);

			return false;
		}

	}
	else
	{
		Application::state = Application::Driving;

		PlSound->play2D(startCar);

		return true;
	}
}