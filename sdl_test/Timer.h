#ifndef _TIMER_H
#define _TIMER_H


const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 1.5f;

class Timer
{
public:
	//static Timer* s_Instance;
	float m_DeltaTime;
	float m_LastTime;
	Timer();
	void Tick();
	inline float GetDeltaTime() { return m_DeltaTime; }

	//static Timer* GetInstance() {

	//	if (s_Instance != nullptr)
	//	{
	//		return s_Instance;
	//	}
	//	else
	//	{
	//		s_Instance = new Timer();
	//			return s_Instance;
	//	}

	//	//return s_Instance - (s_Instance != nullptr)? s_Instance : new Timer();
	//}

private:
	
	

};
#endif
