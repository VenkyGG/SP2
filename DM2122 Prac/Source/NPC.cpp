#include "NPC.h"

// max -1000 x and z

NPC::NPC(int seed)
{
	srand(seed);


	type = rand() % 4;
	
	ismoving = false;
	Position = Vector3(rand() % 1000 - 500, 12, rand() % 1000 - 500);
	Target = Position;

	steps = 0;
	waittime = 0;
	chattime = 0;

	NPCRotationalValue = rand() % 360;
	
}

NPC::~NPC()
{
}

void NPC::setNPCRotation(float degreesNPC)
{
	NPCRotationalValue = degreesNPC;
}


float NPC::getNPCRotation()
{
	return NPCRotationalValue;
}

Vector3 NPC::GetPosition()
{
	return Position;
}

void NPC::SetPosition(Vector3 pos)
{
	Position = pos;
}


int NPC::Gettype()
{
	return type;
}

void NPC::Settype(int x)
{
	type = x;
}



Mesh* NPC::GetMesh(int index)
{
	return CharacterPartsOBJ[index];
}

void NPC::SetMesh(Mesh* x, int index)
{
	
	CharacterPartsOBJ[index] = new Mesh("BODYPART");
	
	CharacterPartsOBJ[index]->mode = x->mode;
	CharacterPartsOBJ[index]->colorBuffer = x->colorBuffer;
	CharacterPartsOBJ[index]->vertexBuffer = x->vertexBuffer;
	CharacterPartsOBJ[index]->indexBuffer = x->indexBuffer;
	CharacterPartsOBJ[index]->ColisionVector1 = x->ColisionVector1;
	CharacterPartsOBJ[index]->ColisionVector2 = x->ColisionVector2;
	CharacterPartsOBJ[index]->collisionboxcreated = x->collisionboxcreated;
	CharacterPartsOBJ[index]->collison = x->collison;
	CharacterPartsOBJ[index]->textureID = x->textureID;
	CharacterPartsOBJ[index]->material = x->material;
	CharacterPartsOBJ[index]->indexSize = x->indexSize;
}

bool NPC::GetIsMoving()
{
	return ismoving;
}
void NPC::move()
{	
	//cout << GetTickCount64() << endl;
	if (waittime < GetTickCount64() && !ismoving)
	{
		ismoving = true;
		srand(time(NULL));
		Target = Vector3(rand() % 1000-500, 10, rand() % 1000-500);
		Target = (Target - Position).Normalized();
		NPCRotationalValue = atan2(Target.x, Target.z) * 180 / Math::PI;
	}
	if (ismoving)
	{
		Position.x += Target.x;
		Position.z += Target.z;
		steps++;
		if ((Target - Position).Length() < 2 || steps==40)
		{
			ismoving = false;
			steps = 0;
			float i = (rand() % 1500 + 500)/100;
			waittime = GetTickCount64() + i * 1000;
		}
	}
}

bool NPC::chat(Vector3 CamPos)
{
	if (chattime < GetTickCount64() && (CamPos - Position).Length() <= 20)
	{
		chattime = GetTickCount64() + 10000;
		return true;
	}
	else if (chattime < GetTickCount64() && (CamPos - Position).Length() > 20)
	{
		return false;
	}
	else if (chattime > GetTickCount64() && (chattime - 5000) > GetTickCount64())
	{
		return true;
	}
	else if (chattime > GetTickCount64() && (chattime - 5000) < GetTickCount64())
	{
		return false;
	}
	
}
