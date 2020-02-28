#ifndef DODGECAR_H
#define DODGECAR_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Application.h"

class DodgeCar : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_TEXT,
		//delete later
		/*GEO_PLAYBUTTON,
		GEO_SETTINGSBUTTON,
		GEO_EXITBUTTON,
		GEO_GAMENAME,
		GEO_MENU,
		GEO_MENUCURSOR,*/

		//new shit
		GEO_LANES,
		GEO_MCAR1,
		GEO_MCAR2,
		GEO_MCAR3,
		GEO_CURTAIN,
		GEO_WALL,
		GEO_WALL2,

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

	float Car1Z = -50.f;
	float Car2Z = -50.f;
	float Car3Z = -50.f;

	int Dodgepoints = 0.f;
	float LosetextY = -10.f;
	float WintextY = -10.f;
	float timer = 0.f;

	float Car1Timer;
	float Car2Timer;
	float Car3Timer;
	//pos.z += (float)(LSPEED * dt);
	//pos.z -= (float)(LSPEED * dt);

	bool Carsmove = false;
	bool Gamelose = false;

	Camera2 camera;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	//void RenderFramerate(Mesh* mesh, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);

public:
	DodgeCar();
	~DodgeCar();


	float pos = 2;
	int level = 1;
	double clock;
	bool Paused;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif