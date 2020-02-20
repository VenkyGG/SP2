#pragma once
#include <Vector3.h>
#include <string>
#include "MeshBuilder.h"
#include "Mesh.h"
#include "Application.h"
using namespace std;
class Object
{
private:
	Vector3 Position;
	Vector3 Rotation;
	Mesh* mesh;
	int NumberOfOccurances;
	string type;
	
public:
	Object();
	Vector3 GetPostition();
	void SetPosition(Vector3 pos);
	Vector3 GetRotation();
	void SetRotation(Vector3 rot);
	Mesh* GetMesh();
	void SetMesh(string name, string filelocation);
	int GetNumberOfOccurences();
	void SetNumberOfOccurences(int number);
	string GetType();
	void SetType(string type);

};

