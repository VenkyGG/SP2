
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Scene.h"
#include <irrKlang.h>
#pragma comment(lib,"irrKlang.lib")

using namespace std;
class Application
{
public:
	enum States
	{
		Mainmenu,
		intro,
		Motorshow,
		Driving,
		PreviewxD,
		Luckyspin,
		Slotmachine,
		DodgeCars,
		Settings,
		TOTALSCENES
	};
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static double getmouseXpos();
	static double getmouseYpos();
	static void mouseupdate();
	static int state;
	static int state2;
	static bool TimeToExit;
	Scene* Ptr[TOTALSCENES];
private:
	//Declare a window object
	StopWatch m_timer;
};

#endif