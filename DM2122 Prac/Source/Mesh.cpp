
#include "Mesh.h"
#include "GL\glew.h"

/******************************************************************************/
/*!
\brief
Default constructor - generate VBO/IBO here

\param meshName - name of mesh
*/
/******************************************************************************/
Mesh::Mesh(const std::string& meshName)
	: name(meshName)
	, mode(DRAW_TRIANGLES)
	, textureID(0)
{
	// Generate buffers
	collison = false;
	collisionboxcreated = false;
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &indexBuffer); //generate index buffer
}

/******************************************************************************/
/*!
\brief
Destructor - delete VBO/IBO here
*/
/******************************************************************************/
Mesh::~Mesh()
{
	// Cleanup VBO here
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &indexBuffer);

	if (textureID > 0) glDeleteTextures(1, &textureID);
}

/******************************************************************************/
/*!
\brief
OpenGL render code
*/
/******************************************************************************/
void Mesh::Render()
{
	// Render VBO here
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glEnableVertexAttribArray(2); // 3rd attribute buffer : normals

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (textureID > 0) {
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}

	if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0);
	else if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, 0);
	else if (mode == DRAW_FAN)
		glDrawElements(GL_TRIANGLE_FAN, indexSize, GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

	if (textureID > 0) { glDisableVertexAttribArray(3); }
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}
void Mesh::Render(unsigned offset, unsigned count)
{
	// Render VBO here
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glEnableVertexAttribArray(2); // 3rd attribute buffer : normals

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (textureID > 0) {
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}

	if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else if (mode == DRAW_FAN)
		glDrawElements(GL_TRIANGLE_FAN, indexSize, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));

	if (textureID > 0) { glDisableVertexAttribArray(3); }
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}