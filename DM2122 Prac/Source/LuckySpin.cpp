#include "LuckySpin.h"
#include "GL\glew.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <iomanip>

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
irrklang::ISoundSource* Spinbgm = engine->addSoundSourceFromFile("Sound//SpinBGM.mp3");//spinning scene bgm
irrklang::ISound* SpinB = engine->play2D(Spinbgm, true, true, true, false);
irrklang::ISoundSource* SpinW = engine->addSoundSourceFromFile("Sound//Spinning.wav");//spinning sound
irrklang::ISound* Wheel = engine->play2D(SpinW, true, true, true, false);

LuckySpin::LuckySpin()
{

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

LuckySpin::~LuckySpin()
{
}

void LuckySpin::Init()
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

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_SPINNINGWHEEL] = MeshBuilder::GenerateOBJ("spinningWheel", "OBJ//spinningWheel.obj");
	meshList[GEO_SPINNINGWHEEL]->textureID = LoadTGA("Image//spinningWheel.tga");

	meshList[GEO_SPINNINGBASE] = MeshBuilder::GenerateOBJ("spinningWheelBase", "OBJ//spinningWheelBase.obj");
	meshList[GEO_SPINNINGBASE]->textureID = LoadTGA("Image//spinningWheelBase.tga");

	timer = 0;
	Spinned = false;
	returnMotor = false;
	SpinnerRotation = 0;
}

void LuckySpin::Update(double dt)
{
	timer += dt;

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

	int offset = 6;
	speed = 8.f;
	//Audio
	if (Spinned == true)
	{
		SpinB->setIsPaused(true);
		Wheel->setIsPaused(false);
	}
	else if (Spinned == false)
	{
		SpinB->setIsPaused(false);
		Wheel->setIsPaused(true);
	}

	if ((Application::IsKeyPressed('B')) && timer < GetTickCount64()) //&&only inthis scene then can press spinner)
	{
		Spinned = true;
		srand(time(NULL));
		finalspin = rand() % 720 + 1440;
		remainder = fmod(finalspin, 360);
		segment = (int(remainder) / 45) + 1;
		//std::cout << " finalspin " << finalspin << "remainder " << remainder << "segment " << segment << std::endl;

	}
	if (Spinned == true && SpinnerRotation < finalspin)
	{
		SpinnerRotation += speed;

		if (SpinnerRotation > finalspin)
		{
			SpinnerRotation = finalspin;
		}
	}
	if (Spinned && SpinnerRotation == finalspin)
	{
		Spinned = false;
		if (segment == 1)
		{
			//+50K
			Player::instance()->addMoney(50000);
			//std::cout << "add 50000" << std::endl;
		}
		//if (segment == 2)
		//{
		//	//exit
		//}
		if (segment == 3)
		{
			//+30K
			Player::instance()->addMoney(30000);
			//std::cout << "add 30000" << std::endl;
		}
		if (segment == 4)
		{
			//+20K
			Player::instance()->addMoney(20000);
			//std::cout << "add 20000" << std::endl;
		}
		if (segment == 5)
		{
			//+10K
			Player::instance()->addMoney(10000);
			//std::cout << "add 10000" << std::endl;
		}
		if (segment == 6)
		{
			//+40K
			Player::instance()->addMoney(40000);
			//std::cout << "add 40000" << std::endl;
		}
		//if (segment == 7)
		//{
		//	//exit
		//}
		if (segment == 8)
		{
			//spin again
			resetRotation();
		}
		if ((Application::IsKeyPressed('N')) && timer < GetTickCount64() && Spinned == false)
		{
			//EXIT
		}
		if (Application::IsKeyPressed(VK_RETURN) || Spinned == false)
		{
			Application::state = Application::Motorshow;
			returnMotor = true;
		}
		if (Application::IsKeyPressed(VK_RETURN) || returnMotor == true)
		{
			SpinB->setIsPaused(true);
			Wheel->setIsPaused(true);
		}

	}
	//reset
	if ((Application::IsKeyPressed('X')) && timer < GetTickCount64())
	{
		SpinnerRotation = 0;

	}
}
void LuckySpin::resetRotation()
{
	SpinnerRotation = 0;
	returnMotor = false;
}

void LuckySpin::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z - 7, camera.target.x, camera.target.y + 2, camera.target.z, camera.up.x, camera.up.y + 10, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	


	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_SPINNINGBASE], false);

	modelStack.PushMatrix();
	modelStack.Translate(0.f, 3.5f, 0.f);
	modelStack.Rotate(SpinnerRotation, 1, 0, 0);
	RenderMesh(meshList[GEO_SPINNINGWHEEL], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	if ((segment == 1 || segment == 3 || segment == 4 || segment == 5 || segment == 6 || segment == 3) && (Spinned == false))
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Congrats!", Color(0.5, 0.5, 0.5), 7.f, 2.f, 5.f);

		if (segment == 1)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "You won $50000", Color(1.f, 1.f, 1.f), 5.f, 1.5f, 5.f);
		}
		if (segment == 3)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "You won $30000", Color(1.f, 1.f, 1.f), 5.f, 1.5f, 5.f);
		}
		if (segment == 4)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "You won $20000", Color(1.f, 1.f, 1.f), 5.f, 1.5f, 5.f);
		}
		if (segment == 5)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "You won $10000", Color(1.f, 1.f, 1.f), 5.f, 1.5f, 5.f);
		}
		if (segment == 6)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "You won $40000", Color(1.f, 1.f, 1.f), 5.f, 1.5f, 5.f);
		}


	}
	else if ((segment == 2 || segment == 7) && Spinned == false)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "BOO!", Color(0.5f, 0.5f, 0.5f), 7.f, 4.f, 5.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Try the", Color(1.f, 1.f, 1.f), 5.f, 4.f, 6.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "next time!", Color(1.f, 1.f, 1.f), 5.f, 2.5f, 5.f);
	}
	else if (segment == 8 && Spinned == false)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press B", Color(1.f, 1.f, 1.f), 7.f, 4.f, 5.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "to Spin again!", Color(1.f, 1.f, 1.f), 5.f, 2.f, 4.f);
	}
	else if (Spinned == false || SpinnerRotation == 0)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press B", Color(1.f, 1.f, 1.f), 7.f, 2.f, 5.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "to Spin", Color(1.f, 1.f, 1.f), 7.f, 2.f, 4.f);
	}
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(Player::instance()->getMoney()), Color(1, 1, 1), 3, 1.f, 18.f);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press 'Enter' to leave", Color(1, 1, 1), 3, 3.f, 1.f);

}

void LuckySpin::Exit()
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

void LuckySpin::RenderMesh(Mesh* mesh, bool enableLight)
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

void LuckySpin::RenderText(Mesh* mesh, std::string text, Color color)
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

void LuckySpin::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void LuckySpin::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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