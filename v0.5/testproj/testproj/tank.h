#pragma once

#include "GameObject.h"
#include "Globals.h"

//--Class for the player tanks

class Tank : public GameObject
{
private:
	int score;
public:
	Tank(int ID);
	void Destroy();

	int gunDelay;

	void Init(float x,float y);
	void Update();
	void Render(HDC deviceContext);

	void MoveUp();
	bool UpPressed;

	void MoveDown();
	bool DownPressed;

	void MoveLeft();
	bool LeftPressed;

	void MoveRight();
	bool RightPressed;

	bool shields;
	bool CheckShields() {return shields;}
	void SetShields(bool shields) {Tank::shields = shields;}

	int Powerup;
	int CheckPowerup() {return Powerup;}
	void SetPowerup(int powerup) {Tank::Powerup = powerup;}

	int Powerupdelay;
	int GetPowerupDelay() {return Powerupdelay;}
	void SetPowerupDelay(int Powerupdelay) {Tank::Powerupdelay = Powerupdelay;}

	void ResetMovementX();
	void ResetMovementY();

	int GetScore() {return score;}
	int GetGunDelay() {return gunDelay;}
	void SetGunDelay(int gunDelay) {Tank::gunDelay = gunDelay;}

	void AddPoint() {score++;}

	void Collided(int objectID);
};