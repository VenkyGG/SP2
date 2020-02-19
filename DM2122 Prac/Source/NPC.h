#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <Vector3.h>

class NPC
{
private:
	float NPCRotationalValue;

	float NPCTranslationXValue;

	float NPCTranslationZValue;

	std::string npcFileHead;
	std::string npcFileBody;
	std::string npcFileLeftArm;
	std::string npcFileRightArm;
	std::string npcFileLeftLeg;
	std::string npcFileRightLeg;
	Vector3 ColisionVector1Storage;
	Vector3 ColisionVector2Storage;
	
public:
	NPC(float seed);
	~NPC();

	void setNPCRotation(float degreesNPC);
	void setNPCTranslationX(float XCoordsNPC);
	void setNPCTranslationZ(float ZCoordsNPC);

	float getNPCRotation();
	float getNPCTranslationX();
	float getNPCTranslationZ();

	std::string getNpcFileHead();
	std::string getNpcFileBody();
	std::string getNpcFileLeftArm();
	std::string getNpcFileRightArm();
	std::string getNpcFileLeftLeg();
	std::string getNpcFileRightLeg();

	Vector3 GetCollisionStorage1();
	void SetCollisionStorage1(Vector3 vector3);
	Vector3 GetCollisionStorage2();
	void SetCollisionStorage2(Vector3 vector3);
};

