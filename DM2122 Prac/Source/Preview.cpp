
#include "Preview.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Physics.h"
#include "Player.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

Preview::Preview()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

Preview::~Preview()
{
}

void Preview::Init()
{
	initialized = true;
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0, 24, 50), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.RotationEnabled = false;
	camera.useWASD = true;
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 50000.f);
	projectionStack.LoadMatrix(projection);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	//For First Light
	for (int i = 0; i < numlights; i++)
	{
		m_parameters[8 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].position_cameraspace").c_str());
		m_parameters[9 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].color").c_str());
		m_parameters[10 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].power").c_str());
		m_parameters[11 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].kC").c_str());
		m_parameters[12 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].kL").c_str());
		m_parameters[13 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].kQ").c_str());
		m_parameters[14 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].type").c_str());
		m_parameters[15 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].spotDirection").c_str());
		m_parameters[16 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].cosCutoff").c_str());
		m_parameters[17 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].cosInner").c_str());
		m_parameters[18 + i * 11] = glGetUniformLocation(m_programID, ("lights[" + to_string(i) + "].exponent").c_str());
	}
	//For Second Light

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Initialize First light
	for (int i = 0; i < numlights; i++)
	{
		light[i].type = Light::LIGHT_SPOT;
		light[i].position.Set(0, 5, 0);
		light[i].color.Set(0.5f, 0.5f, 0.5f);
		light[i].power = 100;
		light[i].kC = 1.f;
		light[i].kL = 0.01f;
		light[i].kQ = 0.001f;
		light[i].cosCutoff = cos(Math::DegreeToRadian(45));
		light[i].cosInner = cos(Math::DegreeToRadian(30));
		light[i].exponent = 3.f;
		light[i].spotDirection.Set(0.f, 1.f, 0.f);
		glUniform3fv(m_parameters[9 + i * 11], 1, &light[i].color.r);
		glUniform1f(m_parameters[10 + i * 11], light[i].power);
		glUniform1f(m_parameters[11 + i * 11], light[i].kC);
		glUniform1f(m_parameters[12 + i * 11], light[i].kL);
		glUniform1f(m_parameters[13 + i * 11], light[i].kQ);
		glUniform1i(m_parameters[14 + i * 11], light[i].type);
		glUniform3fv(m_parameters[15 + i * 11], 1, &light[i].spotDirection.x);
		glUniform1f(m_parameters[16 + i * 11], light[i].cosCutoff);
		glUniform1f(m_parameters[17 + i * 11], light[i].cosInner);
		glUniform1f(m_parameters[18 + i * 11], light[i].exponent);
	}
	glUniform1i(m_parameters[U_NUMLIGHTS], numlights);

	//skybox outdoor
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//hills_lf.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//hills_rt.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//hills_up.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("grass", Color(0, 104.f / 255.f, 0), 5000.f, 5000.f); // THIS CHANGES COLOR OF FLOOR
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOTTOM]->material.kShininess = 1.f;


	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//hills_ft.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//hills_bk.tga");

	//texts
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_MONEYTEXT] = MeshBuilder::GenerateText("MoneyText", 16, 16);
	meshList[GEO_MONEYTEXT]->textureID = LoadTGA("Image//moneyFont.tga");

	// Arrows
	meshList[GEO_LEFTARROW] = MeshBuilder::GenerateQuad("LeftArrow", Color(0.f, 0.f, 0.f), 1, 1);
	meshList[GEO_LEFTARROW]->textureID = LoadTGA("Image//leftArrow.tga");

	meshList[GEO_RIGHTARROW] = MeshBuilder::GenerateQuad("RightArrow", Color(0.f, 0.f, 0.f), 1, 1);
	meshList[GEO_RIGHTARROW]->textureID = LoadTGA("Image//rightArrow.tga");

	//renders crosshair in the middle of screen
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateOBJ("crosshair", "OBJ//crosshair.obj");

	meshList[GEO_EXTRASHAPE1] = MeshBuilder::GenerateOBJ("sun", "OBJ//Cars//ChengFengcar.obj");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);

	

	vector<Mesh*> MeshStorage;

	srand(time(NULL));
	for (int i = 0; i < Player::instance()->cars.GetnumberofCars(); i++)
	{
		Player::instance()->cars.GetCar(i)->SetPosition(0, Vector3(0, 0, 0));
		Player::instance()->cars.GetCar(i)->SetRotation(0, Vector3(0, 0, 0));
	}
}


void Preview::Update(double dt)
{
	if (Application::IsKeyPressed(0x31))
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x32))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	if (Application::IsKeyPressed(VK_LEFT) && bouncetime<GetTickCount64()) // This will change to the previous Car
	{
		if (Player::instance()->cars.GetCurrentCar() != Player::instance()->cars.GetStart())
		{
			Player::instance()->cars.SetCurrentCar(Player::instance()->cars.GetCurrentCar()->Getprevious());

			bouncetime = GetTickCount64() + 500;
		}
	}

	if (Application::IsKeyPressed(VK_RIGHT) && bouncetime < GetTickCount64()) // This will change to the next Car
	{
		if (Player::instance()->cars.GetCurrentCar()->GetNext() != nullptr)
		{
			Player::instance()->cars.SetCurrentCar(Player::instance()->cars.GetCurrentCar()->GetNext());

			bouncetime = GetTickCount64() + 500;
		}
	}
	
	if (Application::IsKeyPressed(VK_RETURN) && bouncetime < GetTickCount64()) // This is the shop function.
	{
		User2.buyCar(Player::instance()->cars.GetCurrentCar());

		bouncetime = GetTickCount64() + 500;
	}

	if (Application::IsKeyPressed('V')) // Change to the Main Menu
	{
		Application::state = Application::Mainmenu;
	}

	camera.target = Player::instance()->cars.GetCurrentCar()->GetPostition()[0];
	camera.Update(dt);
}

void Preview::Render()
{

	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	
	for (size_t i = 0; i < numlights; i++)
	{
		if (light[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[i].position.x, light[i].position.y, light[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[8 + i * 11], 1, &lightDirection_cameraspace.x);
		}
		// if it is spot light, pass in position and direction 
		else if (light[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			glUniform3fv(m_parameters[8 + i * 11], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[i].spotDirection;
			glUniform3fv(m_parameters[15 + i * 11], 1, &spotDirection_cameraspace.x);
		}
		else if (light[i].type == Light::LIGHT_POINT)
		{
			// default is point light (only position since point light is 360 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			glUniform3fv(m_parameters[8 + i * 11], 1, &lightPosition_cameraspace.x);
		}
	}

	RenderSkybox();

	modelStack.PushMatrix();
	modelStack.Translate(starepoint.x, starepoint.y, starepoint.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(Player::instance()->cars.GetCurrentCar()->GetPostition()[0].x, Player::instance()->cars.GetCurrentCar()->GetPostition()[0].y, Player::instance()->cars.GetCurrentCar()->GetPostition()[0].z);
	modelStack.Rotate(Player::instance()->cars.GetCurrentCar()->GetRotation()[0].y, 0, 1, 0);
	RenderMesh(Player::instance()->cars.GetCurrentCar()->GetMeshList()[0], false, true);
	modelStack.PopMatrix();

	if (Player::instance()->cars.GetCurrentCar()->GetNext() != nullptr)
	{
		RenderMeshOnScreen(meshList[GEO_RIGHTARROW], 55, 7, 20, 20, 0);
	}

	if (Player::instance()->cars.GetCurrentCar() != Player::instance()->cars.GetStart())
	{
		RenderMeshOnScreen(meshList[GEO_LEFTARROW], 23, 7, 20, 20, 0);
	}


		
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 40, 30, 2, 2,0); // This renders the crosshair. REMOVE WHEN FINALISING
	RenderFramerate(meshList[GEO_TEXT], Color(0, 0, 0), 3, 21, 19);

	RenderTextOnScreen(meshList[GEO_MONEYTEXT], ("Money:$" + to_string(Player::instance()->getMoney())), Color(0, 0, 0), 2, 0.5, 28.5f); // This prints the Money the player has onto the top left of the Screen

	renderPrice();

	//RenderTextOnScreen(meshList[GEO_TEXT], (":" + std::to_string(plantlist.sun)), Color(0, 0, 0), 5, 2, 10.5f);//render amount of sun in inventory
}

void Preview::renderPrice()
{
	if (!Player::instance()->cars.GetCurrentCar()->GetOwned())
	{
		string hello = "$" + to_string(Player::instance()->cars.GetCurrentCar()->GetPrice());

		if (hello == "$0")
		{
			hello = "FREE";
		}

		RenderTextOnScreen(meshList[GEO_TEXT], "Price:" + hello, Color(0, 0, 0), 5, 2, 10.5f);
	}
	else
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "OWNED", Color(0, 0, 0), 5, 2, 10.5f);
	}

}

void Preview::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void Preview::RenderMesh(Mesh* mesh, bool enableLight, bool hasCollision)
{

	if (hasCollision)
	{
		
		mesh->ColisionVector3 = modelStack.Top().GetTranspose().Multiply(mesh->ColisionVector1);
		mesh->ColisionVector4 = modelStack.Top().GetTranspose().Multiply(mesh->ColisionVector2);
		mesh->collison = true;
		mesh->collisionboxcreated = true;
		

		/*Mesh* Collider = MeshBuilder::GenerateCollisonBox("COLLISIONBOX", mesh->p1, mesh->p2, mesh->p3, mesh->p4, mesh->p5, mesh->p6, mesh->p7, mesh->p8);
		modelStack.PushMatrix();
		RenderMesh(Collider, false, false);
		modelStack.PopMatrix();*/

	}
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);


	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once in the whole function

	if (mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void Preview::RenderSkybox()
{
	float size = bordersize;//uniform scaling
	float offset = size / 200;//used to prevent lines appearing
	
	modelStack.PushMatrix();
	///scale, translate, rotate
	modelStack.Translate(-size + offset, 0.f, 0.f);
	modelStack.Scale(size * 2, size * 2, size * 2);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false, false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(size - offset, 0.f, 0.f);
	modelStack.Scale(size * 2, size * 2, size * 2);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false, false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate
	modelStack.Translate(0.f, size - offset, 0.f);
	modelStack.Scale(size * 2, size * 2, size * 2);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_TOP], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], true, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	///scale, translate, rotate
	modelStack.Translate(0.f, 0.f, -size + offset);
	modelStack.Scale(size * 2, size * 2, size * 2);
	RenderMesh(meshList[GEO_FRONT], false, false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 0.f, size - offset);
	modelStack.Scale(size * 2, size * 2, size * 2);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false, false);
	modelStack.PopMatrix();
	
	
}

void Preview::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Preview::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
void Preview::RenderFramerate(Mesh* mesh, Color color, float size, float x, float y)
{

	static float framesPerSecond = 0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount64() * 0.001f;
	++framesPerSecond;

	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
	std::string frames = "FPS:" + std::to_string(fps);
	if (!mesh || mesh->textureID <= 0)
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < frames.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)frames[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void Preview::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, float rotation)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Rotate(rotation, 0, 0, 1);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false, false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}