#include "SceneSlots.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Player.h"
#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

SceneSlots::SceneSlots()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneSlots::~SceneSlots()
{
}

void SceneSlots::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(1, 1, 0), Vector3(0, 1, 0), Vector3(0, 1, 0));

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

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 5, 0);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 3;
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


	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_CHAR] = MeshBuilder::GenerateQuad("char", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_CHAR]->textureID = LoadTGA("Image//char.tga");

	meshList[GEO_SPINNER] = MeshBuilder::GenerateOBJ("roller", "OBJ//slotspinner.obj");
	meshList[GEO_SPINNER]->textureID = LoadTGA("Image//slotspinner.tga");

	meshList[GEO_SLOTHANDLE] = MeshBuilder::GenerateOBJ("handle", "OBJ//slothandle.obj");
	meshList[GEO_SLOTHANDLE]->textureID = LoadTGA("Image//slothandle.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_SLOTMACHINE] = MeshBuilder::GenerateOBJ("slotmachine", "OBJ//slotmachine.obj");
	meshList[GEO_SLOTMACHINE]->textureID = LoadTGA("Image//slotmachine.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);

	gameStart = false;
}

void SceneSlots::Update(double dt)
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
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('Q'))
	{
		//to do: switch light type to POINT and pass the information to
		light[0].type = Light::LIGHT_POINT;

	}
	else if (Application::IsKeyPressed('W'))
	{
		//to do: switch light type to DIRECTIONAL and pass the
		light[0].type = Light::LIGHT_DIRECTIONAL;

	}
	else if (Application::IsKeyPressed('E'))
	{
		//to do: switch light type to SPOT and pass the information to
		light[0].type = Light::LIGHT_SPOT;

	}


	//camera.Update(dt);

	if (Application::IsKeyPressed('L') && rotate1 == false && rotate2 == false && rotate3 == false)
	{
		rotate1 = true;
		rotate2 = true;
		rotate3 = true;
		threesame = false;
		twosame = false;
		nonesame = false;
		gameStart = true;
		gameEnd = false;
		handleforward = true;
		handleback = false;

	}
	if (Application::IsKeyPressed('1'))
	{
		rotate1 = false;

	}
	else if (Application::IsKeyPressed('2'))
	{
		rotate2 = false;
	}
	else if (Application::IsKeyPressed('3'))
	{
		rotate3 = false;
	}

	if (handleforward)
	{
		rotatehandle2 += 1;
		if (rotatehandle2 > 45)
		{
			rotatehandle2 = 0;
			handleforward = false;
		}
	}
	if (rotate1 == true)
	{
		rotateslot1 += (float)(1500 * dt);
		if (rotateslot1 >= 360)
			rotateslot1 = 0;
	}
	else if (rotate1 == false)
	{
		if (rotateslot1 <= 20 || rotateslot1 >= 340)
		{
			rotateslot1 = 0;
			facenum[0] = 1;
		}
		else if (rotateslot1 <= 60 && rotateslot1 >= 20)
		{
			rotateslot1 = 40;
			facenum[0] = 2;
		}
		else if (rotateslot1 <= 100 && rotateslot1 >= 60)
		{
			rotateslot1 = 80;
			facenum[0] = 3;
		}
		else if (rotateslot1 <= 140 && rotateslot1 >= 100)
		{
			rotateslot1 = 120;
			facenum[0] = 4;
		}
		else if (rotateslot1 <= 180 && rotateslot1 >= 140)
		{
			rotateslot1 = 160;
			facenum[0] = 5;
		}
		else if (rotateslot1 <= 220 && rotateslot1 >= 180)
		{
			rotateslot1 = 200;
			facenum[0] = 6;
		}
		else if (rotateslot1 <= 260 && rotateslot1 >= 220)
		{
			rotateslot1 = 240;
			facenum[0] = 7;
		}
		else if (rotateslot1 <= 300 && rotateslot1 >= 260)
		{
			rotateslot1 = 280;
			facenum[0] = 8;
		}
		else if (rotateslot1 <= 340 && rotateslot1 >= 300)
		{
			rotateslot1 = 320;
			facenum[0] = 9;
		}
	}
	if (rotate2 == true)
	{
		rotateslot2 += (float)(1500 * dt);
		if (rotateslot2 >= 360)
			rotateslot2 = 0;
	}
	else if (rotate2 == false)
	{
		if (rotateslot2 <= 20 || rotateslot2 >= 340)
		{
			rotateslot2 = 0;
			facenum[1] = 1;
		}
		else if (rotateslot2 <= 60 && rotateslot2 >= 20)
		{
			rotateslot2 = 40;
			facenum[1] = 2;
		}
		else if (rotateslot2 <= 100 && rotateslot2 >= 60)
		{
			rotateslot2 = 80;
			facenum[1] = 3;
		}
		else if (rotateslot2 <= 140 && rotateslot2 >= 100)
		{
			rotateslot2 = 120;
			facenum[1] = 4;
		}
		else if (rotateslot2 <= 180 && rotateslot2 >= 140)
		{
			rotateslot2 = 160;
			facenum[1] = 5;
		}
		else if (rotateslot2 <= 220 && rotateslot2 >= 180)
		{
			rotateslot2 = 200;
			facenum[1] = 6;
		}
		else if (rotateslot2 <= 260 && rotateslot2 >= 220)
		{
			rotateslot2 = 240;
			facenum[1] = 7;
		}
		else if (rotateslot2 <= 300 && rotateslot2 >= 260)
		{
			rotateslot2 = 280;
			facenum[1] = 8;
		}
		else if (rotateslot2 <= 340 && rotateslot2 >= 300)
		{
			rotateslot2 = 320;
			facenum[1] = 9;
		}
	}
	if (rotate3 == true)
	{
		rotateslot3 += (float)(1500 * dt);
		if (rotateslot3 >= 360)
			rotateslot3 = 0;
	}
	else if (rotate3 == false)
	{
		if (rotateslot3 <= 20 || rotateslot3 >= 340)
		{
			rotateslot3 = 0;
			facenum[2] = 1;
		}
		else if (rotateslot3 <= 60 && rotateslot3 >= 20)
		{
			rotateslot3 = 40;
			facenum[2] = 2;
		}
		else if (rotateslot3 <= 100 && rotateslot3 >= 60)
		{
			rotateslot3 = 80;
			facenum[2] = 3;
		}
		else if (rotateslot3 <= 140 && rotateslot3 >= 100)
		{
			rotateslot3 = 120;
			facenum[2] = 4;
		}
		else if (rotateslot3 <= 180 && rotateslot3 >= 140)
		{
			rotateslot3 = 160;
			facenum[2] = 5;
		}
		else if (rotateslot3 <= 220 && rotateslot3 >= 180)
		{
			rotateslot3 = 200;
			facenum[2] = 6;
		}
		else if (rotateslot3 <= 260 && rotateslot3 >= 220)
		{
			rotateslot3 = 240;
			facenum[2] = 7;
		}
		else if (rotateslot3 <= 300 && rotateslot3 >= 260)
		{
			rotateslot3 = 280;
			facenum[2] = 8;
		}
		else if (rotateslot3 <= 340 && rotateslot3 >= 300)
		{
			rotateslot3 = 320;
			facenum[2] = 9;
		}
	}
	std::cout << rotateslot1 << " " << rotateslot2 << " " << rotateslot3 << std::endl;
	if (rotate1 == false && rotate2 == false && rotate3 == false && gameStart == true) {
		if (facenum[0] == facenum[1] && facenum[1] == facenum[2])
		{
			threesame = true;
			twosame = false;
			nonesame = false;
		}
		else if (facenum[0] == facenum[1] || facenum[1] == facenum[2] || facenum[0] == facenum[2])
		{
			twosame = true;
			threesame = false;
			nonesame = false;
		}
		else
		{
			nonesame = true;
			twosame = false;
			threesame = false;
		}
	}


	if (threesame == true || twosame == true || nonesame == true)
	{
		gameEnd = true;
	}
}


void SceneSlots::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneSlots::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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


void SceneSlots::Render()
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
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	
	modelStack.PushMatrix();
	modelStack.Translate(-37.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_SLOTMACHINE], false);

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -6.f);
	modelStack.Rotate(-90*sin(Math::DegreeToRadian(rotatehandle2)*4), 0, 0, 1);
	RenderMesh(meshList[GEO_SLOTHANDLE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 7.5, 2.2);
	modelStack.Rotate(rotateslot1, 0, 0, 1);
	RenderMesh(meshList[GEO_SPINNER], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 7.5, 0);
	modelStack.Rotate(rotateslot2, 0, 0, 1);
	RenderMesh(meshList[GEO_SPINNER], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 7.5, -2.2);
	modelStack.Rotate(rotateslot3, 0, 0, 1);
	RenderMesh(meshList[GEO_SPINNER], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
	

	if (threesame == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "three in a row", Color(0, 1, 0), 3, 0, 0);
		Player::instance()->addMoney(500);
	}
	else if (twosame == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "two in a row", Color(0, 1, 0), 3, 0, 0);
		Player::instance()->addMoney(250);
	}
	else if(nonesame == true)
		RenderTextOnScreen(meshList[GEO_TEXT], "you lose", Color(0, 1, 0), 3, 0, 0);

}

void SceneSlots::Exit()
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


void SceneSlots::RenderMesh(Mesh * mesh, bool enableLight)
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