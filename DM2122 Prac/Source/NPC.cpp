#include "NPC.h"

#include "GL\glew.h"
#include "Application.h"


// max -1000 x and z

NPC::NPC(int seed)
{
	srand(seed);


	type = rand() % 4;
	
	

	NPCTranslationXValue = rand() % 2000 - 1000;

	NPCTranslationZValue = rand() % 2000 - 1000;

	NPCRotationalValue = rand() % 360;
}

NPC::~NPC()
{
}

void NPC::setNPCRotation(float degreesNPC)
{
	NPCRotationalValue = degreesNPC;
}

void NPC::setNPCTranslationX(float XCoordsNPC)
{
	NPCTranslationXValue = XCoordsNPC;
}

void NPC::setNPCTranslationZ(float ZCoordsNPC)
{
	NPCTranslationZValue = ZCoordsNPC;
}

float NPC::getNPCRotation()
{
	return NPCRotationalValue;
}

float NPC::getNPCTranslationX()
{
	return NPCTranslationXValue;
}

float NPC::getNPCTranslationZ()
{
	return NPCTranslationZValue;
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
	CharacterPartsOBJ[index] = x;
}

NPC* NPC::GetNext()
{
	return Next;
}

NPC* NPC::Getprevious()
{
	return Previous;
}

void NPC::SetNext(NPC* x)
{
	Next = x;
}

void NPC::SetPrevious(NPC* x)
{
	Previous = x;
}