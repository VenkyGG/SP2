#include "SceneSettings.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "MainMenu.h"
#include "Player.h"
#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

SceneSetting::SceneSetting()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneSetting::~SceneSetting()
{
}

void SceneSetting::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(40, 1, 0), Vector3(0, 1, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Blending.fragmentshader");

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0,1].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");



	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);


	meshList[GEO_SETTINGMENU] = MeshBuilder::GenerateQuad("menu", Color(0.f, 0.63f, 0.48f), 50.f, 50.f);

	meshList[GEO_POINTER] = MeshBuilder::GenerateQuad("pointer", Color(1.f, 1.f, 1.f), 10.f, 10.f);
	meshList[GEO_POINTER]->textureID = LoadTGA("Image//MainMenu Textures//Arrow.tga");

	meshList[GEO_SETTING] = MeshBuilder::GenerateQuad("setting", Color(1.f, 0.f, 0.f), 20.f, 20.f);

	meshList[GEO_AUDIOBUTTON] = MeshBuilder::GenerateQuad("audiobutton", Color(1, 0, 0), 15.f, 15.f);

	meshList[GEO_BUTTON] = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 10.f, 10.f);
	meshList[GEO_BUTTON]->textureID = LoadTGA("Image//switchon.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");


	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);



	button = true;
}
double MainMenu::clock2 = 0;
void SceneSetting::Update(double dt)
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
	

	float offsety = 9.5;     //used to change the y axis of the pointer
	float offsetx = 3.5;     //used to change the x axis of the pointer
	if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && clock2 < GetTickCount64() && level < 2)       //if S is pressed, pointer move down. clock2 to delay moving up and down time so that it cannot be spammed
	{
		pos -= offsety;    //change the axis of the pointer
		pos2 += offsetx;
		level++;
		clock2 = GetTickCount64() + 400;
	}
	else if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && clock2 < GetTickCount64() && level > 1) //if W is pressed, move pointer up. clock2 to delay moving up and down time so that it cannot be spammed
	{
		pos += offsety;   //change the axis of the pointer
		pos2 -= offsetx;
		level--;
		clock2 = GetTickCount64() + 400;
	}

	//camera.Update(dt);

	if (level == 1 && clock < GetTickCount64() && (Application::IsKeyPressed(VK_LBUTTON) || Application::IsKeyPressed(VK_RETURN))) //if pointer is at the first option and enter is pressed
	{
		
		clock = GetTickCount64() + 400;
		if (button == true)             //if button is on, turn it off
		{
			Player::instance()->Audio = false;
			meshList[GEO_BUTTON]->textureID = LoadTGA("Image//switchoff.tga");   //switching the texture of the button from on to off
			button = false;
		}
		else if (button == false)      //if button is on, turn it off
		{
			Player::instance()->Audio = true;
			meshList[GEO_BUTTON]->textureID = LoadTGA("Image//switchon.tga");    //switching the texture of the button from off to on
			button = true;
		}
	}
	else if (level == 2 && clock3 < GetTickCount64() && (Application::IsKeyPressed(VK_LBUTTON) || Application::IsKeyPressed(VK_RETURN)))//if pointer is at the second option and enter is pressed
	{
		MainMenu::clock2 = GetTickCount64() + 500;
		Application::state = Application::Mainmenu;   //change scene to main menu
	}


}


void SceneSetting::RenderText(Mesh* mesh, std::string text, Color color)
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
		characterSpacing.SetToTranslation(i * 0.6f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneSetting::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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


void SceneSetting::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_SETTINGMENU], false);       //rendering background

	modelStack.PushMatrix();
	modelStack.Translate(pos2, pos, 1.f);
	modelStack.Scale(0.3f, 0.3f, 0.f);
	RenderMesh(meshList[GEO_POINTER], false);       //rendering pointer
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.f, -2.f, 1.f);
	modelStack.Scale(0.5f, 0.2f, 0.f);
	RenderMesh(meshList[GEO_BUTTON], false);        //rendering switch button
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], "SETTINGS", Color(0, 0, 0), 4, 6.f, 13.f);   //render text settings

	RenderTextOnScreen(meshList[GEO_TEXT], "Audio:", Color(0, 0, 0), 3, 10.f, 10.f);    //render text audio

	RenderTextOnScreen(meshList[GEO_TEXT], "EXIT", Color(0, 0, 0), 3, 12.f, 4.f);       //render text exit

	if (button == true)                                                                  //if the button is on
		RenderTextOnScreen(meshList[GEO_TEXT], "ON", Color(0, 0, 0), 3, 17.f, 10.f);    //render text on
	else if (button == false)                                                            //if the button if off
		RenderTextOnScreen(meshList[GEO_TEXT], "OFF", Color(0, 0, 0), 3, 17.f, 10.f);   //render text off

}

void SceneSetting::Exit()
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


void SceneSetting::RenderMesh(Mesh * mesh, bool enableLight)
{
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
