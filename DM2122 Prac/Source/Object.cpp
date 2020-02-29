#include "Object.h"

Object::Object()
{
	meshlist.push_back(new Mesh("INITIAL"));
	Vector3 position = Vector3(0, 0, 0);
	PositionList.push_back(position);
	Vector3 rotation = Vector3(0, 0, 0);
	RotationList.push_back(rotation);
	NumberOfOccurances = 1;
	type = "Object";
}

void Object::Delete()
{
	delete mesh;
	mesh = NULL;
	for (auto& q : meshlist)
	{
		delete q;
		q = NULL;
	}
	for (auto& q : meshlist)
	{
		if (q!=NULL)
		cout << q->name << endl;
	}
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
	for (int i = 0; i < NumberOfOccurances; i++)
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
		meshlist[i]->initColisionVector1 = mesh->initColisionVector1;
		meshlist[i]->initColisionVector2 = mesh->initColisionVector2;
		meshlist[i]->initColisionVector3 = mesh->initColisionVector3;
		meshlist[i]->initColisionVector4 = mesh->initColisionVector4;
		meshlist[i]->ColisionVector1 = mesh->ColisionVector1;
		meshlist[i]->ColisionVector2 = mesh->ColisionVector2;
		meshlist[i]->ColisionVector3 = mesh->ColisionVector3;
		meshlist[i]->ColisionVector4 = mesh->ColisionVector4;
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
	meshlist[0]->mode = mesh->mode;
	meshlist[0]->colorBuffer = mesh->colorBuffer;
	meshlist[0]->vertexBuffer = mesh->vertexBuffer;
	meshlist[0]->indexBuffer = mesh->indexBuffer;
	meshlist[0]->ColisionVector1 = mesh->ColisionVector1;
	meshlist[0]->ColisionVector2 = mesh->ColisionVector2;
	meshlist[0]->collisionboxcreated = mesh->collisionboxcreated;
	meshlist[0]->collison = mesh->collison;
	meshlist[0]->textureID = mesh->textureID;
	meshlist[0]->material = mesh->material;
	meshlist[0]->indexSize = mesh->indexSize;
	meshlist[0]->initColisionVector1 = mesh->initColisionVector1;
	meshlist[0]->initColisionVector2 = mesh->initColisionVector2;
	meshlist[0]->initColisionVector3 = mesh->initColisionVector3;
	meshlist[0]->initColisionVector4 = mesh->initColisionVector4;
	meshlist[0]->ColisionVector1 = mesh->ColisionVector1;
	meshlist[0]->ColisionVector2 = mesh->ColisionVector2;
	meshlist[0]->ColisionVector3 = mesh->ColisionVector3;
	meshlist[0]->ColisionVector4 = mesh->ColisionVector4;
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
		x->initColisionVector1 = mesh->initColisionVector1;
		x->initColisionVector2 = mesh->initColisionVector2;
		x->initColisionVector3 = mesh->initColisionVector3;
		x->initColisionVector4 = mesh->initColisionVector4;
		x->ColisionVector1 = mesh->ColisionVector1;
		x->ColisionVector2 = mesh->ColisionVector2;
		x->ColisionVector3 = mesh->ColisionVector3;
		x->ColisionVector4 = mesh->ColisionVector4;
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


