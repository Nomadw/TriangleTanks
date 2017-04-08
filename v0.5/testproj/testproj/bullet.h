#pragma once

#include "GameObject.h"

//Class for the bullet

class Bullet : public GameObject
{
private:
	void (*ScorePoint)(void);

public:
	Bullet(float x, float y, void(*ScorePoint)(void));
	Bullet(float x, float y);
	Bullet(float x, float y, int ID);
	Bullet::Bullet(float x, float y, int ID, float x_dir, float y_dir);
	void Destroy();

	void Update();
	void Render(HDC deviceContext);
	//void Render();

	void Collided(int objectID);
};