#include "Object.h"

Object::Object()
{
	Mesh* x = new Mesh("INITIAL");
	meshlist.push_back(x);
	Vector3 position = Vector3(0, 0, 0);
	PositionList.push_back(position);
	Vector3 rotation = Vector3(0, 0, 0);
	RotationList.push_back(rotation);
	NumberOfOccurances = 1;
	type = "Object";
}

vector<Vector3> Object::GetPostition()
{
	return PositionList;
}

void Object::SetPosition(int index, Vector3 pos)
{
	PositionList[index].x = pos.x;
	PositionList[index].y = pos.y;
	PositionList[index].z = pos.z;
}

vector<Vector3> Object::GetRotation()
{
	return RotationList;
}

void Object::SetRotation(int index, Vector3 rot)
{
	RotationList[index].x = rot.x;
	RotationList[index].y = rot.y;
	RotationList[index].z = rot.z;
}

Mesh* Object::GetMesh()
{
	return mesh;
}

void Object::SetMesh(string name, string filelocation)
{
	mesh = MeshBuilder::GenerateOBJ(name, filelocation);
	for (size_t i = 0; i < NumberOfOccurances; i++)
	{
		meshlist[i]->mode = mesh->mode;
		meshlist[i]->colorBuffer = mesh->colorBuffer;
		meshlist[i]->vertexBuffer = mesh->vertexBuffer;
		meshlist[i]->indexBuffer = mesh->indexBuffer;
		meshlist[i]->ColisionVector1 = mesh->ColisionVector1;
		meshlist[i]->ColisionVector2 = mesh->ColisionVector2;
		meshlist[i]->collisionboxcreated = mesh->collisionboxcreated;
		meshlist[i]->collison = mesh->collison;
		meshlist[i]->textureID = mesh->textureID;
		meshlist[i]->material = mesh->material;
		meshlist[i]->indexSize = mesh->indexSize;
	}

}

vector<Mesh*> Object::GetMeshList()
{
	return meshlist;
}

int Object::GetNumberOfOccurences()
{
	return NumberOfOccurances;
}

void Object::SetNumberOfOccurences(int number)
{
	NumberOfOccurances = number;
	for (int i = 0; i < number-1; i++)
	{
		Mesh* x = new Mesh("COPY");
		x->mode = mesh->mode;
		x->colorBuffer = mesh->colorBuffer;
		x->vertexBuffer = mesh->vertexBuffer;
		x->indexBuffer = mesh->indexBuffer;
		x->ColisionVector1 = mesh->ColisionVector1;
		x->ColisionVector2 = mesh->ColisionVector2;
		x->collisionboxcreated = mesh->collisionboxcreated;
		x->collison = mesh->collison;
		x->textureID = mesh->textureID;
		x->material = mesh->material;
		x->indexSize = mesh->indexSize;
		meshlist.push_back(x);
		PositionList.push_back(Vector3(0, 0, 0));
		RotationList.push_back(Vector3(0, 0, 0));
	}
}

string Object::GetType()
{
	return type;
}

void Object::SetType(string type)
{
	this->type = type;
}


