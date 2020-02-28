#ifndef DrivingScene_H
#define DrivingScene_H

#include "Scene.h"
#include <MatrixStack.h>
#include "ThirdPersonCam.h"
#include "Mesh.h"
#include "Light.h"
#include "NPC.h"
#include "CarsList.h"

class DrivingScene : public Scene
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
		GEO_LIGHTSPHERE,
		GEO_SPEEDOMETERBACK,
		GEO_SPEEDOMETERFRONT,
		GEO_TEXT,
		GEO_CROSSHAIR,
		NUM_GEOMETRY,
	};
	const static int numlights2 = 40;


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


		U_NUMLIGHTS = 10000,
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

	Light light[numlights2];
	Object objectlist[40];


	int numberofobjects;

	ThirdPersonCamera camera;
	float bordersize = 500.0f;
	float angledetector;
	float innerradius;
	float outerradius;

	bool CheckSquareCollision();
	void RenderMesh(Mesh* mesh, bool enableLight, bool hasCollision);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderFramerate(Mesh* mesh, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, float rotation);

	Vector3 starepoint;
public:
	StopWatch Drivetimer;
	float timenow;
	int moneyToAdd;
	bool timeToAdd;
	float moneyYpos;
	DrivingScene();
	~DrivingScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif