#pragma once

#include "GameObject.h"

//--Class for the in-game powerups

class PowerupS : public GameObject
{
public:
	PowerupS(float x, float y, int ID);
	void Destroy();

	void Update();
	void Render(HDC deviceContext);

	void Collided(int objectID);
};