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
	
	Vector3 ColisionVector1;
	Vector3 ColisionVector2;
	Vector3 ColisionVector3;
	Vector3 ColisionVector4;
	Vector3 initColisionVector1;
	Vector3 initColisionVector2;
	Vector3 initColisionVector3;
	Vector3 initColisionVector4;
	bool camcollided = false;
	Vector3 camfreezeposition;
	
	bool collison;
	bool collisionboxcreated;
	Material material;
	unsigned textureID;
};

#endif