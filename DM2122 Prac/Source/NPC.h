#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

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
};

