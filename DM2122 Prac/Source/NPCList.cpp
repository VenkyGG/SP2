#include "NPCList.h"

NPCList::NPCList(int numberofNPCs)
{
	
	NPC* current = NULL;
	for (int i = 0; i < numberofNPCs; i++)
	{
		if (i == 0)
		{
			Start = new NPC(rand()%10000);
		}
		else if (i == 1)
		{
			current = new NPC(rand() % 10000);
			current->SetPrevious(Start);
			Start->SetNext(current);
		}
		else
		{
			NPC* temp = new NPC(rand() % 10000);
			current->SetNext(temp);
			temp->SetPrevious(current);
			current = temp;
		}
		 
	}
}

NPCList::~NPCList()
{

}

void NPCList::SetMesh(int index, Mesh* x)
{
	MeshStorage[index] = x;
}
