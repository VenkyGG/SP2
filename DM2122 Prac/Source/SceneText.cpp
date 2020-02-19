
#include "SceneText.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

#include <numeric>
#include <vector>

using namespace std;

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f


SceneText::SceneText()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	rotateBot = 0.f;
	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0, 10, 10), Vector3(0, 5, 0), Vector3(0, 1, 0));

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
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	//For Second Light
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Initialize First light
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 5, 0);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 0.5f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &light[0].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	//Initialize Second light
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(500, 500, 0);
	light[1].color.Set(0.5f, 0.5f, 0.5f);
	light[1].power = 1.5f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &light[1].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	//skybox outdoor
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//hills_lf.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//hills_rt.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//hills_up.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("grass", Color(0, 104.f / 255.f, 0), 5000.f, 5000.f);
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOTTOM]->material.kShininess = 1.f;


	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//hills_ft.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//hills_bk.tga");

	//skybox Motorshow
	meshList[GEO_MOTORSHOW_WALL] = MeshBuilder::GenerateQuad("wall", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_MOTORSHOW_WALL]->textureID = LoadTGA("Image//wall_texture.tga");

	meshList[GEO_MOTORSHOW_CEILING] = MeshBuilder::GenerateQuad("ceiling", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_MOTORSHOW_CEILING]->textureID = LoadTGA("Image//ceilingtexture.tga");
	//Flatland surrounding field
	meshList[GEO_FLATLAND] = MeshBuilder::GenerateQuad("flatland", Color(1, 1, 1), 2000.f, 2000.f);
	meshList[GEO_FLATLAND]->textureID = LoadTGA("Image//floortexture.tga");
	meshList[GEO_FLATLAND]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FLATLAND]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLATLAND]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_FLATLAND]->material.kShininess = 1.f;


	//texts
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//renders crosshair in the middle of screen
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateOBJ("crosshair", "OBJ//crosshair.obj");
	//meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//peashooter.tga");
	

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);


	srand(time(NULL));
	CCar* current = cars.GetStart();
	for (int i = 0; i < cars.GetnumberofCars(); i++)
	{
		current->GetMesh()->textureID = LoadTGA(current->getTextureLocation().c_str());
		if (i != cars.GetnumberofCars() - 1)
		{
			current = current->GetNext();
		}
	}
	for (int i = 0; i <numbots; ++i)
	{
		Bot[i] = new NPC(rand() % 10000);

		
	}


	meshList[GEO_HEAD] = MeshBuilder::GenerateOBJ("Head", Bot[2]->getNpcFileHead());
	meshList[GEO_HEAD]->textureID = LoadTGA("Image//People Textures//Woman1.tga");
	meshList[GEO_BODY] = MeshBuilder::GenerateOBJ("Body", Bot[2]->getNpcFileBody());
	meshList[GEO_BODY]->textureID = LoadTGA("Image//People Textures//Woman1.tga");
	meshList[GEO_LEFTARM] = MeshBuilder::GenerateOBJ("Left Arm", Bot[2]->getNpcFileLeftArm());
	meshList[GEO_LEFTARM]->textureID = LoadTGA("Image//People Textures//Woman1.tga");
	meshList[GEO_RIGHTARM] = MeshBuilder::GenerateOBJ("Right Arm", Bot[2]->getNpcFileRightArm());
	meshList[GEO_RIGHTARM]->textureID = LoadTGA("Image//People Textures//Woman1.tga");
	meshList[GEO_LEFTLEG] = MeshBuilder::GenerateOBJ("Left Leg", Bot[2]->getNpcFileLeftLeg());
	meshList[GEO_LEFTLEG]->textureID = LoadTGA("Image//People Textures//Woman1.tga");
	meshList[GEO_RIGHTLEG] = MeshBuilder::GenerateOBJ("Right Leg", Bot[2]->getNpcFileRightLeg());
	meshList[GEO_RIGHTLEG]->textureID = LoadTGA("Image//People Textures//Woman1.tga");
	OutsideMotorShow = true;//set time to day

	
}


void SceneText::Update(double dt)
{

	//std::cout << "Cursor Position at (" << Application << " : ";// <<  << std::endl;
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
	light[0].power = 0;
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	if (!OutsideMotorShow)//if night
	{
		//light[0].position.x = 0;//set light to above tile selector
		//light[0].position.y = 5;
		//light[0].position.z = 0;
		//light[0].power = 1;
		//light[1].power = 0.5f;
		//light[1].color.Set(0, 0, 0.5f);//make light slightly blue
		//glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		//glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		//glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}
	else//if day
	{
		/*light[0].power = 0;
		light[1].power = 1.5f;
		light[1].color.Set(0.5f, 0.5f, 0.5f);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);*/
	}



	float xoffset = -14.5f;//x offset of top left hand corner
	float zoffset = -24;//z offset of top left hand corner
	Vector3 Target2 = camera.position;
	int maxdistance = 1000;
	for (int i = 0; i < maxdistance; i++)
	{
		Target2 += camera.view;//raycast to 20 times player direction
		if (Target2.y <= 0)
		{
			break;
		}
	}


	starepoint = Target2;
	float speed = 2;
	if (Application::IsKeyPressed('Z') && TimeChangeDelay < GetTickCount64())//changes night and day
	{
		OutsideMotorShow = !OutsideMotorShow;//day is not day
		TimeChangeDelay = GetTickCount64() + 250;//delay so day wont be spammed
	}
	if (Application::IsKeyPressed('W'))
	{

		camera.position = camera.position + camera.view * speed;
		camera.position.y = camera.playerheight;
		CheckSquareCollision();
		camera.target = camera.position + camera.view;

	}
	if (Application::IsKeyPressed('S'))
	{

		camera.position = camera.position - camera.view * speed;
		camera.position.y = camera.playerheight;
		CheckSquareCollision();
		camera.target = camera.position + camera.view;

	}
	if (Application::IsKeyPressed('A'))
	{
		camera.position = camera.position - camera.right * speed;
		CheckSquareCollision();
		camera.target = camera.position + camera.view;
	}
	if (Application::IsKeyPressed('D'))
	{
		camera.position = camera.position + camera.right * speed;
		CheckSquareCollision();
		camera.target = camera.position + camera.view;
	}

	rotateBot += (float)(1.f + dt);

	camera.Update(dt);


}

void SceneText::Render()
{

	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_POINT)
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}


	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_POINT)
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	RenderSkybox();

	modelStack.PushMatrix();
	modelStack.Translate(starepoint.x, starepoint.y, starepoint.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false, false);
	modelStack.PopMatrix();

	if (!OutsideMotorShow)//Renders Motorshow stuff
	{
		for (int i = 0; i < numbots; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(Bot[i]->getNPCTranslationX(), 10, Bot[i]->getNPCTranslationZ());
			modelStack.Rotate(Bot[i]->getNPCRotation(), 0, 1, 0);
			Bot[i]->SetCollisionStorage1(modelStack.Top().GetTranspose().Multiply(meshList[GEO_BODY]->ColisionVector1));
			Bot[i]->SetCollisionStorage2(modelStack.Top().GetTranspose().Multiply(meshList[GEO_BODY]->ColisionVector2));
			meshList[GEO_BODY]->collison = true;
			RenderMesh(meshList[GEO_BODY], false, true);

				modelStack.PushMatrix();
			
				RenderMesh(meshList[GEO_HEAD], false, true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
			
				modelStack.Rotate(sin(rotateBot/25)*50, 1.f, 0.f, 0.f);
				RenderMesh(meshList[GEO_LEFTARM], false, true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Rotate(-sin(rotateBot/25)*50, 1.f, 0.f, 0.f);
				RenderMesh(meshList[GEO_RIGHTARM], false, true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, -4.5f, 0);
				modelStack.Rotate(-sin(rotateBot / 25) * 50, 1.f, 0.f, 0.f);
				RenderMesh(meshList[GEO_LEFTLEG], false, true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0, -4.5f, 0);
				modelStack.Rotate(sin(rotateBot / 25) * 50, 1.f, 0.f, 0.f);
				RenderMesh(meshList[GEO_RIGHTLEG], false, true);
				modelStack.PopMatrix();


			modelStack.PopMatrix();

			
		}
		if (cars.GetnumberofCars() != 0)
		{
			CCar* current = cars.GetStart();
			for (int i = 0; i < cars.GetnumberofCars(); i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(current->getxLocation(), 0, current->getzLocation());
				if (current->GetIsSpinning())
				{
					modelStack.Rotate(GetTickCount64() * 0.01f, 0.f, 1.f, 0.f);
				}
				current->GetMesh()->collison= false;
				RenderMesh(current->GetMesh(), false, true);
				modelStack.PopMatrix();
				if (i != cars.GetnumberofCars() - 1)
				{
					current = current->GetNext();
				}
			}
		}
	}

	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 40, 30, 2, 2);//render crosshair
	RenderFramerate(meshList[GEO_TEXT], Color(0, 0, 0), 3, 21, 19);
	//RenderTextOnScreen(meshList[GEO_TEXT], (":" + std::to_string(plantlist.sun)), Color(0, 0, 0), 5, 2, 10.5f);//render amount of sun in inventory
}

void SceneText::Exit()
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

void SceneText::CheckSquareCollision()
{
	for (int current = GEO_AXES + 1; current != NUM_GEOMETRY; current++)
	{
		if (meshList[current] != NULL)
		{ 
			if (meshList[current]->collison)
			{
				if (current == GEO_BODY)
				{
					for (int i = 0; i < numbots; i++)
					{
						float xmin = Bot[i]->GetCollisionStorage2().x;
						float xmax = Bot[i]->GetCollisionStorage1().x;
						float ymin = Bot[i]->GetCollisionStorage2().y;
						float ymax = Bot[i]->GetCollisionStorage1().y;
						float zmin = Bot[i]->GetCollisionStorage2().z;
						float zmax = Bot[i]->GetCollisionStorage1().z;

						if (camera.position.x <= xmax && camera.position.z <= zmax && camera.position.z >= zmin && abs(camera.position.x - xmax) <= 2)
						{
							camera.position.x = xmax + 0.1f;

						}
						if (camera.position.x >= xmin && camera.position.z <= zmax && camera.position.z >= zmin && abs(camera.position.x - xmin) <= 2)
						{
							camera.position.x = xmin - 0.1f;
						}
						if (camera.position.z <= zmax && camera.position.x <= xmax && camera.position.x >= xmin && abs(camera.position.z - zmax) <= 2)
						{
							camera.position.z = zmax + 0.1f;
						}
						if (camera.position.z >= zmin && camera.position.x <= xmax && camera.position.x >= xmin && abs(camera.position.z - zmin) <= 2)
						{
							camera.position.z = zmin - 0.1f;
						}
					}
				}
				else
				{
					float xmin = meshList[current]->ColisionVector2.x;
					float xmax = meshList[current]->ColisionVector1.x;
					float ymin = meshList[current]->ColisionVector2.y;
					float ymax = meshList[current]->ColisionVector1.y;
					float zmin = meshList[current]->ColisionVector2.z;
					float zmax = meshList[current]->ColisionVector1.z;
					//cout << xmax << endl;

					if (camera.position.x <= xmax && camera.position.z <= zmax && camera.position.z >= zmin && abs(camera.position.x - xmax) <= 2)
					{
						camera.position.x = xmax + 0.1f;
					}
					if (camera.position.x >= xmin && camera.position.z <= zmax && camera.position.z >= zmin && abs(camera.position.x - xmin) <= 2)
					{
						camera.position.x = xmin - 0.1f;
					}
					if (camera.position.z <= zmax && camera.position.x <= xmax && camera.position.x >= xmin && abs(camera.position.z - zmax) <= 2)
					{
						camera.position.z = zmax + 0.1f;
					}
					if (camera.position.z >= zmin && camera.position.x <= xmax && camera.position.x >= xmin && abs(camera.position.z - zmin) <= 2)
					{
						camera.position.z = zmin - 0.1f;
					}
					
				}
				meshList[current]->collison = false;
			}
		}
	}
}

void SceneText::RenderMesh(Mesh* mesh, bool enableLight, bool hasCollision)
{
	if (hasCollision)
	{
		if (mesh != meshList[GEO_BODY])
		{
			if (!mesh->collisionboxcreated)
			{
				mesh->ColisionVector1 = modelStack.Top().GetTranspose().Multiply(mesh->ColisionVector1);
				mesh->ColisionVector2 = modelStack.Top().GetTranspose().Multiply(mesh->ColisionVector2);
				mesh->collison = true;
				mesh->collisionboxcreated = true;
			}
		}
		//Mesh* Collider = MeshBuilder::GenerateCollisonBox("COLLISIONBOX", mesh->p1, mesh->p2, mesh->p3, mesh->p4, mesh->p5, mesh->p6, mesh->p7, mesh->p8);
		//modelStack.PushMatrix();
		//RenderMesh(Collider, false, false);
		//modelStack.PopMatrix();

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

void SceneText::RenderSkybox()
{
	float size = 1000;//uniform scaling
	float offset = size / 200;//used to prevent lines appearing
	if (OutsideMotorShow)//render daytime skybox
	{
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
	else//render motorshow skybox
	{
		modelStack.PushMatrix();
		///scale, translate, rotate
		modelStack.Translate(-size + offset, 0.f, 0.f);
		modelStack.Scale(size * 2, size * 2, size * 2);
		modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
		RenderMesh(meshList[GEO_MOTORSHOW_WALL], false, true);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(size - offset, 0.f, 0.f);
		modelStack.Scale(size * 2, size * 2, size * 2);
		modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
		RenderMesh(meshList[GEO_MOTORSHOW_WALL], false, true);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		///scale, translate, rotate
		modelStack.PushMatrix();
		///scale, translate, rotate
		modelStack.Translate(0.f, 0.f, -size + offset);
		modelStack.Scale(size * 2, size * 2, size * 2);
		RenderMesh(meshList[GEO_MOTORSHOW_WALL], false, true);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		///scale, translate, rotate 
		modelStack.Translate(0.f, 0.f, size - offset);
		modelStack.Scale(size * 2, size * 2, size * 2);
		modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
		RenderMesh(meshList[GEO_MOTORSHOW_WALL], false, true);
		modelStack.PopMatrix();
		modelStack.Translate(0.f, size - offset, 0.f);
		modelStack.Scale(size * 2.75f, size * 2.75f, size * 2.75f);
		modelStack.Rotate(GetTickCount64() * 0.01f, 0.f, 1.f, 0.f);
		modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
		RenderMesh(meshList[GEO_MOTORSHOW_CEILING], false, true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(-90, 1, 0, 0);
		RenderMesh(meshList[GEO_FLATLAND], true, true);
		modelStack.PopMatrix();
	}
}

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void SceneText::RenderFramerate(Mesh* mesh, Color color, float size, float x, float y)
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

void SceneText::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false, false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

//float scalarTriple(Vector3 u, Vector3 v, Vector3 w)
//{
//	return inner_product((u, v), w);
//}