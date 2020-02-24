#ifndef MESH_H
#define MESH_H

#include <string>
#include "Vertex.h"
#include "Material.h"
#include <Mtx44.h>

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_FAN,
		DRAW_MODE_LAST,
	};

	Mesh(const std::string& meshName);
	~Mesh();
	void Render();
	void Render(unsigned offset, unsigned count);
	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned colorBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	Vector3 p4;
	Vector3 p5;
	Vector3 p6;
	Vector3 p7;
	Vector3 p8;
	Vector3 ColisionVector1;
	Vector3 ColisionVector2;
	Mtx44 inversetransform;
	
	bool collison;
	bool collisionboxcreated;
	Material material;
	unsigned textureID;
};

#endif