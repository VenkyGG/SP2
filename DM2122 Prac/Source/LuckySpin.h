#ifndef LUCKYSPIN_H
#define LUCKYSPIN_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Application.h"
#include "Player.h" 

class LuckySpin : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_TEXT,
		GEO_SPINNINGWHEEL,
		GEO_SPINNINGBASE,
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
	Player* player;
	float pos = 2;
	double timer;
	bool Spinned;
	float SpinnerRotation = 0;
	float finalspin;
	float remainder;
	float segment;
	float speed;

	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;


	Camera2 camera;
	void CalculateSpin();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);

public:
	//bool SpinningWheel;
	LuckySpin();
	~LuckySpin();



	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif#pragma once
