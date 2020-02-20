#pragma once
#include "Mesh.h"
#include "Application.h"
#include <Vector3.h>
using namespace std;
class NPC
{
private:
	float NPCRotationalValue;

	Vector3 Position;


	int type;
	int steps;
	Mesh* CharacterPartsOBJ[6];

	float waittime;
	bool ismoving;
	Vector3 Target;

	float chattime;

public:
	NPC(int seed);
	~NPC();

	void setNPCRotation(float degreesNPC);

	float getNPCRotation();
	Vector3 GetPosition();
	void SetPosition(Vector3 pos);

	int Gettype();
	void Settype(int x);

	Mesh* GetMesh(int index);
	void SetMesh(Mesh* x, int index);

	bool GetIsMoving();

	void move();
	
	bool chat(Vector3 CamPos);
	

};

