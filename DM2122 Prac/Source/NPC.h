#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <time.h>
#include <string>
#include <experimental/filesystem> 
#include "Mesh.h"
using namespace std;
class NPC
{
private:
	float NPCRotationalValue;

	float NPCTranslationXValue;

	float NPCTranslationZValue;


	int type;
	Mesh* CharacterPartsOBJ[6];
	string CharacterTexture;
	int numpeople;

	NPC* Next;
	NPC* Previous;
public:
	NPC(float seed);
	~NPC();

	void setNPCRotation(float degreesNPC);
	void setNPCTranslationX(float XCoordsNPC);
	void setNPCTranslationZ(float ZCoordsNPC);

	float getNPCRotation();
	float getNPCTranslationX();
	float getNPCTranslationZ();

	int Gettype();
	void Settype(int x);

	
	Mesh* GetMesh(int index);
	void SetMesh(Mesh* x, int index);
	

	NPC* GetNext();
	NPC* Getprevious();
	void SetNext(NPC* x);
	void SetPrevious(NPC* x);
};

