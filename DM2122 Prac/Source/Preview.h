#ifndef Preview_H
#define Preview_H

#include "Scene.h"
#include <MatrixStack.h>
#include "ThirdPersonCam.h"
#include "Mesh.h"
#include "Light.h"
#include "NPC.h"
#include "CarsList.h"
#include "Shop.h"
#include "irrKlang.h"


class Preview : public Scene
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
		GEO_SPEEDOMETERBACK,
		GEO_SPEEDOMETERFRONT,
		GEO_TEXT,
		GEO_TEXT2,
		GEO_MONEYTEXT,
		GEO_MONEYSYMBOL,
		GEO_LEFTARROW,
		GEO_RIGHTARROW,
		GEO_CROSSHAIR,
		NUM_GEOMETRY,
	};
	const static int numlights = 0;


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


		U_NUMLIGHTS = 500,
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
	Object objectlist[2];


	int currentcar = 1;
	int numberofobjects;

	float rgbRed;
	float rgbGreen;
	float rgbBlue;

	bool done1;
	bool done2;
	bool done3;
	bool done4;
	bool done5;
	bool done6;

	bool moneyBool;

	double tickDelay;

	bool test;

	double bouncetime;

	ThirdPersonCamera camera;
	float bordersize = 500.0f;

	void RenderMesh(Mesh* mesh, bool enableLight, bool hasCollision);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderFramerate(Mesh* mesh, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, float rotation);

	void renderPrice();

	void RGBChroma(double dt);

	Vector3 starepoint;

	Shop User2;

	bool printNotEnufMoney;
	double printNotEnufMoneyDelay;

public:
	Preview();
	~Preview();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif