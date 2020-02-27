#include "DodgeCar.h"
#include "GL\glew.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

DodgeCar::DodgeCar()
{

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

DodgeCar::~DodgeCar()
{
}

void DodgeCar::Init()
{
	initialized = true;
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0, 1, 1), Vector3(0, 1, 0), Vector3(0, 1, 0));

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

	/*meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_EXITBUTTON] = MeshBuilder::GenerateQuad("exitButton", Color(1, 1, 1), 50.f, 50.f);
	meshList[GEO_EXITBUTTON]->textureID = LoadTGA("Image//MainMenu Textures//ExitBtn.tga");

	meshList[GEO_SETTINGSBUTTON] = MeshBuilder::GenerateQuad("settingsButton", Color(1, 1, 1), 50.f, 50.f);
	meshList[GEO_SETTINGSBUTTON]->textureID = LoadTGA("Image//MainMenu Textures//SettingsBtn.tga");

	meshList[GEO_PLAYBUTTON] = MeshBuilder::GenerateQuad("playButton", Color(1, 1, 1), 50.f, 50.f);
	meshList[GEO_PLAYBUTTON]->textureID = LoadTGA("Image//MainMenu Textures//PlayBtn.tga");

	meshList[GEO_GAMENAME] = MeshBuilder::GenerateQuad("gameName", Color(1, 1, 1), 50.f, 50.f);
	meshList[GEO_GAMENAME]->textureID = LoadTGA("Image//MainMenu Textures//LOGO.tga");

	meshList[GEO_GAMENAME] = MeshBuilder::GenerateQuad("gameName", Color(1, 1, 1), 50.f, 50.f);
	meshList[GEO_GAMENAME]->textureID = LoadTGA("Image//DodgeCar Textures//Pause.tga");

	meshList[GEO_MENUCURSOR] = MeshBuilder::GenerateQuad("menuCursor", Color(0.f, 0.23f, 0.68f), 50.f, 50.f);
	meshList[GEO_MENUCURSOR]->textureID = LoadTGA("Image//MainMenu Textures//Arrow.tga");*/

	/*meshList[GEO_MENU] = MeshBuilder::GenerateQuad("menu", Color(0.f, 0.63f, 0.48f), 50.f, 50.f);*/

	meshList[GEO_LANES] = MeshBuilder::GenerateOBJ("Lanes", "Obj//dodgetrack.obj");
	meshList[GEO_LANES]->textureID = LoadTGA("image//dodgetrack.tga");

	meshList[GEO_MCAR1] = MeshBuilder::GenerateOBJ("Car1", "Obj//Dcar.obj");
	meshList[GEO_MCAR1]->textureID = LoadTGA("image//DcarRed.tga");

	meshList[GEO_MCAR2] = MeshBuilder::GenerateOBJ("Car2", "Obj//Dcar.obj");
	meshList[GEO_MCAR2]->textureID = LoadTGA("image//DcarYellow.tga");

	meshList[GEO_MCAR3] = MeshBuilder::GenerateOBJ("Car3", "Obj//Dcar.obj");
	meshList[GEO_MCAR3]->textureID = LoadTGA("image//DcarBlue.tga");

	meshList[GEO_CURTAIN] = MeshBuilder::GenerateOBJ("Curtain", "Obj//Dodgecarsbackdrop.obj");
	meshList[GEO_CURTAIN]->textureID = LoadTGA("image//Dodgecarsbackdrop.tga");

	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("Wall", "Obj//Dodgecarswall.obj");
	meshList[GEO_WALL]->textureID = LoadTGA("image//Dodgecarsceiling.tga");

	meshList[GEO_WALL2] = MeshBuilder::GenerateOBJ("Wall", "Obj//Dodgecarswall2.obj");
	meshList[GEO_WALL2]->textureID = LoadTGA("image//Dodgecarswall.tga");

	clock = 0;


}
void DodgeCar::Update(double dt)
{
	clock += dt;

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

	float height = Application::getmouseYpos();
	Application::mouseupdate();

	int offset = 6;
	/*if ((Application::IsKeyPressed('S') || height > 0) && clock < GetTickCount64() && level < 3)
	{
		pos -= offset;
		level++;
		clock = GetTickCount64() + 500;
	}
	else if ((Application::IsKeyPressed('W') || height < 0) && clock < GetTickCount64() && level>1)
	{
		pos += offset;
		level--;
		clock = GetTickCount64() + 500;
	}*/
	/*if (Application::IsKeyPressed('A'))
	{
		camera.position.z += dt;
	}
	if (Application::IsKeyPressed('D'))
	{
		camera.position.z -= dt;
	}*/
	float speed = 0.3;
	if (Application::IsKeyPressed('A'))
	{
		camera.position = camera.position - camera.right * speed;
		camera.target = camera.position + camera.view;
		/*camera.position.z = 3.5f;*/
	}
	//if (Application::IsKeyPressed('S'))
	//{
	//	camera.position = camera.position - camera.right * speed;
	//	camera.target = camera.position + camera.view;
	//	/*camera.position.z = 0.f;*/
	//}
	if (Application::IsKeyPressed('D'))
	{
		camera.position = camera.position + camera.right * speed;
		camera.target = camera.position + camera.view;
		/*camera.position.z = -3.5f;*/
	}
	if (Application::IsKeyPressed('X'))
	{
		Carsmove = true;

	}
	if (Application::IsKeyPressed('C'))
	{
		Carsmove = false;
	}

	if (Carsmove == true)
	{
		Car1Timer += dt;
		Car2Timer += dt;
		Car3Timer += dt;

		GameStart += dt;


		if (Car1Timer > 0.1f && Car1Timer <= 1.5f)
		{
			Car1Z += (float)((LSPEED * dt) * 9);
		}
		if (Car1Timer > 1.5f)
		{
			Car1Timer = 0.1f;
			Car1Z = -50.f;
		}

		if (Car2Timer > 0.5f && Car2Timer <= 1.1f)
		{
			Car2Z += (float)((LSPEED * dt) * 9);
		}
		if (Car2Timer > 1.1f)
		{
			Car2Timer = 0.5f;
			Car2Z = -50.f;
		}
		if (Car3Timer > 1.f && Car3Timer <= 2.2f)
		{
			Car3Z += (float)((LSPEED * dt) * 9);
		}
		if (Car3Timer > 2.2f)
		{
			Car3Timer = 0.5f;
			Car3Z = -50.f;
		}
		/*if (timer > 0.5f && timer<= 2.f)
		{
			pos.y += (float)(LSPEED * dt);
		}
		if (timer > 2.f && timer <= 3.5f)
		{
			pos.y -= (float)(LSPEED * dt);
		}
		if (timer > 3.5f)
		{
			timer = 0;
			jumping = false;
		}*/
		if (GameStart > 20.f)
		{
			Carsmove = false;
		}
	}

	if (Carsmove == false)
	{
		Car1Timer = 0;
		Car2Timer = 0;
		Car3Timer = 0;
		GameStart = 0;

		Car1Z = -50.f;
		Car2Z = -50.f;
		Car3Z = -50.f;
	}
	//cout << camera.position.x << endl;
	if ((abs(Car1Z - camera.position.z) <= 5) && (camera.position.x>=-5.5f && camera.position.x<=-1.8f))
	{
		Carsmove = false;
	}
	if ((abs(Car2Z - camera.position.z) <= 5) && (camera.position.x >= -1.8f && camera.position.x <= 1.8f))
	{
		Carsmove = false;
	}
	if ((abs(Car3Z - camera.position.z) <= 5) && (camera.position.x >= 1.8f && camera.position.x <= 5.5f))
	{
		Carsmove = false;
	}

	if (abs(camera.position.x >= 5.5))
	{
		camera.position.x = 5.4;
		camera.target.x = 5.4;
	}
	if (abs(camera.position.x <= -5.5))
	{
		camera.position.x = -5.4;
		camera.target.x = -5.4;
	}
	
	else if (Paused)
	{
		//render paused texture
	}
}



void DodgeCar::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	


	/*modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_MENU], 30.f, 25.f, 2.f, 2.f);

	modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_MENUCURSOR], 20, 2 * pos + 30, 0.2f, 0.2f);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_GAMENAME], 40.f, 50.f, 0.6f, 0.5f);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_PLAYBUTTON], 40.f, 34.f, 0.4f, 0.2f);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_SETTINGSBUTTON], 40.f, 22.f, 0.4f, 0.2f);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_EXITBUTTON], 40.f, 10.f, 0.4f, 0.2f);
	modelStack.PopMatrix();

	modelStack.PopMatrix();*/



	modelStack.PushMatrix();
	modelStack.Translate(0, -2, -25);
	modelStack.Scale(1, 1, 2);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_LANES], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3.5f, -1, Car1Z);
	RenderMesh(meshList[GEO_MCAR1], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -1, Car2Z);
	RenderMesh(meshList[GEO_MCAR2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3.5f, -1, Car3Z);
	RenderMesh(meshList[GEO_MCAR3], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -2, -45);
	modelStack.Scale(1.8f, 1.5f, 2);
	RenderMesh(meshList[GEO_CURTAIN], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 4.4, -25);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(2.07, 1, 9);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-8.9, 1, -30);
	modelStack.Scale(1, 1, 9);
	RenderMesh(meshList[GEO_WALL2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(8.9, 1, -30);
	modelStack.Scale(1, 1, 9);
	RenderMesh(meshList[GEO_WALL2], false);
	modelStack.PopMatrix();

}

void DodgeCar::Exit()
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


void DodgeCar::RenderMesh(Mesh* mesh, bool enableLight)
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

void DodgeCar::RenderText(Mesh* mesh, std::string text, Color color)
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

void DodgeCar::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void DodgeCar::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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
	RenderMesh(mesh, false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}
