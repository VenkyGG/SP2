#include "CarsList.h"



CarsList::CarsList()
{
	int maxspeed = 25;
	CCar* current = Start;
	numcars = 0;
	for (auto& p : std::experimental::filesystem::directory_iterator("OBJ//Cars"))
	{
		if (Start == NULL)
		{
			string location = p.path().filename().string();
			for (int i = 0; i < location.length(); i++)
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
			Start->SetIsSpinning(true);
			Start->setfileLocation("OBJ//Cars//"+location+".obj");
			Start->SetMesh(Start->getfileLocation());
			Start->setTextureLocation("Image//Car Textures//" + location + ".tga");
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
			current->SetIsSpinning(true);
			current->setfileLocation("OBJ//Cars//" + location + ".obj");
			current->SetMesh(current->getfileLocation());
			current->setTextureLocation("Image//Car Textures//" + location + ".tga");
		}
		maxspeed += 25;
	}
	float angleposition = 360.0f / numcars;
	float currentangle = 0;
	current = Start;
	float camxpos = 0;
	float camzpos = 0;
	float radius = 100.0f;
	for (int i = 0; i < numcars; i++)
	{
		current->setxLocation(camxpos + radius * sin(Math::DegreeToRadian(currentangle)));
		current->setzLocation(camzpos + radius * cos(Math::DegreeToRadian(currentangle)));
		currentangle += angleposition;
		if (i != numcars - 1)
		{
			current = current->GetNext();
		}
	}
}

CarsList::~CarsList()
{
	if (numcars == 1)
	{
		delete Start;
	}
	else
	{
		CCar* current = Start;
		for (int i = 0; i < numcars; i++)
		{
			if (i == 0)
			{
				current = Start->GetNext();
				delete Start;
			}
			else if (i != numcars - 1)
			{
				CCar* temp = current->GetNext();
				delete current;
				current = temp;
			}
			else
			{
				delete current;
			}

		}
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
