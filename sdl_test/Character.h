
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Grid.h"
#include "TimerPause.h"



using namespace std;

class Character {

public:
	Transform transform;
	virtual void init();
	virtual void draw();
	virtual void move();
	virtual void update(float deltaTime);
	virtual void SetGridPosition(Vector2 gridPosition);
	Vector2 currentGridPosition;
	
	bool isInDamage = false;
	bool isMoving = false;

private:
	

protected:
	Grid* grid;
	SDL_Texture* characterTex;
	SDL_Texture* characterDamageTex;
	SDL_Rect characterRect;

	TimerPause* timerPauseCoroutine;

	//PARA ANIMACIONES DE DESPLAZAMIENTO
	float tick = 0.0f;
	float duration = 10.0f;
	float startX = 0.0f;
	float endX = 0.0f;
	float startY = 0.0f;
	float endY = 0.0f;
};
#endif