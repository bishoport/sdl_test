#ifndef _TIMER_PAUSE_H
#define _TIMER_PAUSE_H


#include <string>
#include <iostream>
//#include "EnemyCharacter.h"
using namespace std;


/* TYPEDEF */
//typedef int (*funcion_callback);

class TimerPause
{
public:
	

	TimerPause()
	{
	}

	//funcion_callback _func;
	std::function<void()> _func = []() {};

	void StartPause(std::function<void()> func , int duration)
	{
		this->duration = duration;
		this->_func = func;
		isInPause = true;
	}


	virtual void EndPause()
	{
		_func();
	}


	void update(float deltaTime)
	{
		if (isInPause)
		{
			tick += deltaTime;
			//cout << "tick " << tick << endl;
			float tval = min(tick / duration, 1.0f);

			if (tval == 1)
			{
				//finished transition
				isInPause = false;
				tick = 0.0f;
				EndPause();
			}
		}
	}

private:
	bool isInPause = false;
	float tick = 0.0f;
	float duration = 1.0f;
};
#endif