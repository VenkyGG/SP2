#include "CarsList.h"



CarsList::CarsList()
{
	int maxspeed = 25;
	CCar* current = Start;
	numcars = 0;
	for (auto& p : std::experimental::filesystem::recursive_directory_iterator("OBJ//Cars"))
	{
		if (Start == NULL)
		{
			string location = p.path().filename().string();
			for (size_t i = 0; i < location.length(); i++)
			{
				if (location[i] == '.')
				{
					location  = location.substr(0, i);
					break;
				}
			}
			
			Start = new CCar;
			numcars=1;
			
			Start->setmaxSpeed(maxspeed);
			Start->setfileLocation("OBJ//Cars//"+location+".obj");
			Start->setTextureLocation("Image//Car Textures//"+location+".tga");
			
		}
		else
		{
			string location = p.path().filename().string();
			for (size_t i = 0; i < location.length(); i++)
			{
				if (location[i] == '.')
				{
					location = location.substr(0, i);
					break;
				}
			}

			
			numcars++;
			if (numcars == 2)
			{
				current = new CCar;
				current->SetPrevious(Start);
				Start->SetNext(current);
			}
			else
			{
				CCar* temp = new CCar;
				current->SetNext(temp);
				temp->SetPrevious(current);
				current = temp;
			}
			current->setmaxSpeed(maxspeed);
			current->setfileLocation("OBJ//Cars//" + location + ".obj");
			current->setTextureLocation("Image//Car Textures//" + location + ".tga");
		}
		maxspeed += 25;
	}
}

CarsList::~CarsList()
{
	for (int i = 0; i < numcars; i++)
	{

	}
}

int CarsList::GetnumberofCars()
{
	return numcars;
}

CCar* CarsList::GetStart()
{
	return Start;
}

void CarsList::SetStart(CCar* x)
{
	Start = x;
}
