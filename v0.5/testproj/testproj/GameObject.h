#pragma once

// Include headers accessible 
//#include <iostream>
#include "Globals.h"

//--The main GameObject base class. All other game objects/classes inherit from here

class GameObject
{
private:
	
	bool alive;				// On screen (or not)
	bool collidable;		// can collide with other objects

protected:
	float x;				// x - coordinate
	float y;				// y - coordinate

	float x_speed;			// velocity along x axis
	float y_speed;			// velocity along y axis

	float x_dir;				// direction x is moving (left or right)
	float y_dir;				// direction y is moving (up or down)

	float x_bound;			// width of bounding box	
	float y_bound;			// height of bounding box

public:
	// Constructor
	GameObject();
	int Object_ID;			// ID number
	// Virtual methods (may be replaced by 'child' methods)
	void virtual Destroy();
	void virtual Update();
	void virtual Render();
	void virtual Render(HDC deviceContext);
	void virtual Collided(int Object_ID);


	// ACCESSOR methods (reads and returns a value)
	int GetID() {return Object_ID;}
	bool GetAlive() {return alive;}
	bool GetCollidable() {return collidable;}

	float GetX() {return x;}
	float GetY() {return y;}

	float GetX_Bound() {return x_bound;}
	float GetY_Bound() {return y_bound;}

	bool CheckObjectID(int ID);


	float rotation;
	float GetRotation() {return rotation;}
	void SetRotation(float rotation) {GameObject::rotation = rotation;}


	// MUTATOR methods (changes a value)
	void SetID(int ID) {GameObject::Object_ID = ID;}
	void SetAlive(bool alive) {GameObject::alive = alive;}
	void SetCollidable(bool collidable) {GameObject::collidable = collidable;}

	float accelerationX;
	int accelerationDelayX;
	float accelerationY;
	int accelerationDelayY;

	int GetaccelerationDelayX() {return accelerationDelayX;}
	void SetaccelerationDelayX(int accelerationDelayX) {GameObject::accelerationDelayX = accelerationDelayX;}
	int GetaccelerationDelayY() {return accelerationDelayY;}
	void SetaccelerationDelayY(int accelerationDelayY) {GameObject::accelerationDelayY = accelerationDelayY;}
	
	void increaseAccelerationX();
	void decreaseAccelerationX();
	void increaseAccelerationY();
	void decreaseAccelerationY();

	

	void SetX(float x) {GameObject::x = x;}
	void SetY(float y) {GameObject::y = y;}



	// Additional methods
	void Init(float x, float y, float x_speed, float y_speed, float x_dir, float y_dir, float x_bound, float y_bound);
	bool CheckCollisions(GameObject *otherObject);
	bool Collidable();
};