#include "CarsList.h"
#include "LoadTGA.h"
#include "Player.h"

CarsList::CarsList()
{
	int maxspeed = 5;
	int Price = 0;
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
			Start->SetMesh(location, "OBJ//Cars//" + location + ".obj");
			Start->GetMesh()->textureID=LoadTGA(("Image//Car Textures//" + location + ".tga").c_str());
			Start->GetMesh()->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
			Start->GetMesh()->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			Start->GetMesh()->material.kSpecular.Set(1.f, 1.f, 1.f);
			Start->GetMesh()->material.kShininess = 1.f;
			Start->SetType("Car");
			Start->GetMeshList()[0]->textureID = Start->GetMesh()->textureID;
			Start->SetPrice(Price);
		
		}
		else
		{
			string location = p.path().filename().string();
			for (int i = 0; i < location.length(); i++)
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
			current->SetMesh(location, "OBJ//Cars//" + location + ".obj");
			current->GetMesh()->textureID = LoadTGA(("Image//Car Textures//" + location + ".tga").c_str());
			current->GetMesh()->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
			current->GetMesh()->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			current->GetMesh()->material.kSpecular.Set(1.f, 1.f, 1.f);
			current->GetMesh()->material.kShininess = 1.f;
			current->SetType("Car");
			current->SetPrice(Price);
			current->GetMeshList()[0]->textureID = current->GetMesh()->textureID;
			
		}
		maxspeed += 5;
		Price += 100000;
	}
	
	CurrentCar = current;
}

CarsList::~CarsList()
{
	if (numcars == 1)
	{
		Start->Delete();
		delete Start;
	}
	else
	{
		CCar* current = Start;
		for (int i = 0; i < numcars; i++)
		{
			if (i == 0)
			{
				/*current = Start->GetNext();
				Start->Delete();
				delete Start;*/
			}
			 if (i != numcars - 1)
			{
				CCar* temp = current->GetNext();
				current->Delete();
				delete current;
				current = temp;
			}
			else
			{
				current->Delete();
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

CCar* CarsList::GetCar(int index)
{
	CCar* current = Start;
	for (int i = 0; i < numcars; i++)
	{
		if (i == index)
		{
			return current;
		}
		else if (i != numcars - 1)
		{
			current = current->GetNext();
		}
	}
}

CCar* CarsList::GetCurrentCar()
{
	return CurrentCar;
}

void CarsList::SetCurrentCar(CCar* car)
{
	CurrentCar = car;
}
