#pragma once
#include "NPC.h"
#include <vector>
#include "MeshBuilder.h"
#include "LoadTGA.h"
class NPCList
{
private:
	int numNPC = 500;
	vector<Mesh*> MeshStorage;
	vector<string> TextureStorage;
	NPC* Start;
public:
	NPCList(int numberofNPCs);
	~NPCList();
	int GetNumberofNPC();
	void SetMesh(int index, Mesh* x);
	NPC* GetStart();
	void SetStart(NPC* x);
};

