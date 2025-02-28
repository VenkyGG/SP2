﻿#ifndef SCENETEXT_H
#define SCENETEXT_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "NPC.h"
#include "CarsList.h"
#include "Player.h"

#include <irrKlang.h>
class SceneText : public Scene
{

	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_MOTORSHOW_WALL,
		GEO_MOTORSHOW_CEILING,
		GEO_FLOOR,
		GEO_HOLOGRAM1,
		GEO_HOLOGRAM2,
		GEO_HOLOGRAM3,
		GEO_LIGHTSPHERE,
		GEO_TEXT,
		GEO_CROSSHAIR,
		NUM_GEOMETRY,
	};
	const static int numlights = 9;
	const static int numberofNPCs = 300;

	enum Scene5_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,

		
		U_NUMLIGHTS = 500 ,
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

	Light light[numlights];
	NPC NPCs[numberofNPCs];
	Object objectlist[50];

	int numberofobjects;

	Camera2 camera;
	float bordersize = 500.0f;

	float hologramsize1;
	float hologramsize2;
	float hologramsize3;

	void CheckSquareCollision();
	void RenderMesh(Mesh* mesh, bool enableLight, bool hasCollision);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderFramerate(Mesh* mesh, Color color, float size, float x, float y);




	Vector3 starepoint;
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
};

#endif