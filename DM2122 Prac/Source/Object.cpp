#include "Object.h"

Object::Object()
{
	Position = Vector3(0, 0, 0);
	Rotation = Vector3(0, 0, 0);
	NumberOfOccurances = 1;
	type = "Object";
}

Vector3 Object::GetPostition()
{
	return Position;
}

void Object::SetPosition(Vector3 pos)
{
	Position = pos;
}

Vector3 Object::GetRotation()
{
	return Rotation;
}

void Object::SetRotation(Vector3 rot)
{
	Rotation = rot;
}

Mesh* Object::GetMesh()
{
	return mesh;
}

void Object::SetMesh(string name, string filelocation)
{
	mesh = MeshBuilder::GenerateOBJ(name, filelocation);
}

string Object::GetType()
{
	return type;
}

void Object::SetType(string type)
{
	this->type = type;
}
