#ifndef SCENEMAINMENU_H
#define SCENEMAINMENU_H

#include "Source/Scene.h"
#include <MatrixStack.h>
#include "Source/Camera2.h"
#include "Source/Mesh.h"
#include "Source/Light.h"
#include "Source/NPC.h"
#include "Source/CarsList.h"

class SceneMainMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_MOTORSHOW_WALL,
		GEO_MOTORSHOW_CEILING,
		GEO_FLATLAND,
		GEO_LIGHTSPHERE,
		GEO_EXTRASHAPE1,
		GEO_CARPLATFORMBASE,
		GEO_CARPLATFORMTOP,
		GEO_TEXT,
		GEO_BODY,
		GEO_CROSSHAIR,
		GEO_PLAYBUTTON,
		GEO_SETTINGSBUTTON,
		GEO_EXITBUTTON,
		GEO_GAMENAME,
		GEO_MENU,
		GEO_MENUCURSOR,
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
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
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
	Light light[2];
	int numbots = 50;
	NPC* Bot[50];

	CarsList cars;

	Camera2 camera;

	void CheckSquareCollision();
	void RenderMesh(Mesh* mesh, bool enableLight, bool hasCollision);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderFramerate(Mesh* mesh, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	void RenderMenu();
	float pos = 5;
	bool OutsideMotorShow;
	double TimeChangeDelay;
	Vector3 starepoint;
public:
	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif