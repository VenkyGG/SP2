
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
using namespace std;
class Application
{
public:
	enum States
	{
		Mainmenu,
		Motorshow,
		Driving,
		PreviewxD,
		Luckyspin,
		Slotmachine,
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
private:
	//Declare a window object
	StopWatch m_timer;
};

#endif