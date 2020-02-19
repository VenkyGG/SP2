#include "NPC.h"

// max -1000 x and z

NPC::NPC(float seed)
{
	srand(seed);

	declareNPC = 0;

	declareNPC = 2;// rand() % 4 + 1;

	if (declareNPC == 1)
	{
		npcFileHead = "OBJ//Man1//Man1Head.obj";
		npcFileBody = "OBJ//Man1//Man1Body.obj";
		npcFileLeftArm = "OBJ//Man1//Man1LeftArm.obj";
		npcFileRightArm = "OBJ//Man1//Man1RightArm.obj";
		npcFileLeftLeg = "OBJ//Man1//Man1LeftLeg.obj";
		npcFileRightLeg = "OBJ//Man1//Man1RightLeg.obj";
	}

	if (declareNPC == 2)
	{
		npcFileHead = "OBJ//Woman1//Woman1Head.obj";
		npcFileBody = "OBJ//Woman1//Woman1Body.obj";
		npcFileLeftArm = "OBJ//Woman1//Woman1LeftArm.obj";
		npcFileRightArm = "OBJ//Woman1//Woman1RightArm.obj";
		npcFileLeftLeg = "OBJ//Woman1//Woman1LeftLeg.obj";
		npcFileRightLeg = "OBJ//Woman1//Woman1RightLeg.obj";
	}

	if (declareNPC == 3)
	{
		npcFileHead = "OBJ//Man2//Man2Head.obj";
		npcFileBody = "OBJ//Man2//Man2Body.obj";
		npcFileLeftArm = "OBJ//Man2//Man2LeftArm.obj";
		npcFileRightArm = "OBJ//Man2//Man2RightArm.obj";
		npcFileLeftLeg = "OBJ//Man2//Man2LeftLeg.obj";
		npcFileRightLeg = "OBJ//Man2//Man2RightLeg.obj";
	}

	if (declareNPC == 4)
	{
		npcFileHead = "OBJ//Woman2//Woman2Head.obj";
		npcFileBody = "OBJ//Woman2//Woman2Body.obj";
		npcFileLeftArm = "OBJ//Woman2//Woman2LeftArm.obj";
		npcFileRightArm = "OBJ//Woman2//Woman2RightArm.obj";
		npcFileLeftLeg = "OBJ//Woman2//Woman2LeftLeg.obj";
		npcFileRightLeg = "OBJ//Woman2//Woman2RightLeg.obj";
	}

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

std::string NPC::getNpcFileHead()
{
	return npcFileHead;
}

std::string NPC::getNpcFileBody()
{
	return npcFileBody;
}

std::string NPC::getNpcFileLeftArm()
{
	return npcFileLeftArm;
}

std::string NPC::getNpcFileRightArm()
{
	return npcFileRightArm;
}

std::string NPC::getNpcFileLeftLeg()
{
	return npcFileLeftLeg;
}

std::string NPC::getNpcFileRightLeg()
{
	return npcFileRightLeg;
}

Vector3 NPC::GetCollisionStorage1()
{
	return ColisionVector1Storage;
}

void NPC::SetCollisionStorage1(Vector3 vector3)
{
	ColisionVector1Storage = vector3;
}

Vector3 NPC::GetCollisionStorage2()
{
	return ColisionVector2Storage;
}

void NPC::SetCollisionStorage2(Vector3 vector3)
{
	ColisionVector2Storage = vector3;
}

int NPC::getDeclareNPC()
{
	return declareNPC;
}
