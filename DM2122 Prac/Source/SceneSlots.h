#ifndef SCENESLOTS_H
#define SCENESLOTS_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"


class SceneSlots : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		
		GEO_LIGHTSPHERE,
		GEO_TEXT,
		GEO_SLOTTEXT,
		GEO_SPINNER,
		GEO_SLOTHANDLE,
		GEO_SLOTMACHINE,

		NUM_GEOMETRY,
	};

	enum Scene6_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,

		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,


		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	Light light[1];
	float rotateslot1 = 0;
	float rotateslot2 = 0;
	float rotateslot3 = 0;
	bool rotate1 = false;
	bool rotate2 = false;
	bool rotate3 = false;
	int facenum[3];
	bool threesame;
	bool twosame;
	bool nonesame;
	bool threesame2;
	bool twosame2;
	bool nonesame2;
	bool gameStart;
	bool gameEnd = false;
	bool handleforward;
	float rotatehandle;
	bool tmp;
	Camera2 camera;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderFramerate(Mesh* mesh, Color color, float size, float x, float y);

public:
	SceneSlots();
	~SceneSlots();

	

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
