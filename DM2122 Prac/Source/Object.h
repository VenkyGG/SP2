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
	vector<Vector3> PositionList;
	vector<Vector3> RotationList;
	Mesh* mesh;
	vector<Mesh*> meshlist;
	int NumberOfOccurances;
	string type;
	
public:
	Object();
	vector<Vector3> GetPostition();
	void SetPosition(int index, Vector3 pos);
	vector<Vector3> GetRotation();
	void SetRotation(int index, Vector3 rot);
	Mesh* GetMesh();
	void SetMesh(string name, string filelocation);
	vector<Mesh*> GetMeshList();
	int GetNumberOfOccurences();
	void SetNumberOfOccurences(int number);
	string GetType();
	void SetType(string type);



};

