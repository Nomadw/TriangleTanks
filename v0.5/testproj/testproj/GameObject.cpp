#include "GameObject.h"
#include "Globals.h"

//--Functions for the base GameObject class. All other classes inherit from here

// Constructor
GameObject::GameObject()
{
	x = 0.0f;
	y = 0.0f;

	x_speed = 0;
	y_speed = 0;

	x_dir = 0;
	y_dir = 0;

	x_bound = 0;
	y_bound = 0;

	rotation = 0.0f;
	accelerationX = 0;
	accelerationY = 0;

	alive = true;
	collidable = true;
}

void GameObject::Destroy()
{
	// Change 16 - remove code
}

void GameObject::Init(float x, float y, float x_speed, float y_speed, float x_dir, float y_dir, float x_bound, float y_bound)
{
	GameObject::x = x;
	GameObject::y = y;

	GameObject::x_speed = x_speed;
	GameObject::y_speed = y_speed;

	GameObject::x_dir = x_dir;
	GameObject::y_dir = y_dir;

	GameObject::x_bound = x_bound;
	GameObject::y_bound = y_bound;
}

void GameObject::Update()
{
	x += (x_speed + accelerationX) * x_dir;
	y += (y_speed + accelerationY) * y_dir;
}

void GameObject::Render()
{


}


void GameObject::Render(HDC deviceContext)
{


}

bool GameObject::CheckCollisions(GameObject *otherObject)
{
	float oX = otherObject->GetX();
	float oY = otherObject->GetY();

	float obX = otherObject->GetX_Bound();
	float obY = otherObject->GetY_Bound();

	if( x + x_bound > oX - obX &&
		x - x_bound < oX + obX &&
		y + y_bound > oY - obY &&
		y - y_bound < oY + obY)
		return true;
	else
		return false;
}

bool GameObject::CheckObjectID(int ID)
{
	if( Object_ID == ID)
		return true;
	else
		return false;
}

void GameObject::Collided(int objectID)
{}

bool GameObject::Collidable()
{
	return alive && collidable;
}

void GameObject::increaseAccelerationY()
{
	if(accelerationDelayY > 0)
                       accelerationDelayY -= 1;
	if(accelerationDelayY == 0 && accelerationY < ACCELERATION_MAX) {
		accelerationDelayY = ACCELERATION_DELAY_MAX;
		accelerationY += ACCELERATION;
	}
}

void GameObject::increaseAccelerationX()
{
	if(accelerationDelayX > 0)
                       accelerationDelayX -= 1;
	if(accelerationDelayX == 0 && accelerationX < ACCELERATION_MAX) {
		accelerationDelayX = ACCELERATION_DELAY_MAX;
		accelerationX += ACCELERATION;
	}
}

void GameObject::decreaseAccelerationY()
{
	if(accelerationDelayY > 0)
                       accelerationDelayY -= 1;
	if(accelerationDelayY == 0 && accelerationY > 0) {
		accelerationDelayY = ACCELERATION_DELAY_MAX;
		accelerationY -= ACCELERATION;
	}
}

void GameObject::decreaseAccelerationX()
{
	if(accelerationDelayX > 0)
                       accelerationDelayX -= 1;
	if(accelerationDelayX == 0 && accelerationX > 0) {
		accelerationDelayX = ACCELERATION_DELAY_MAX;
		accelerationX -= ACCELERATION;
	}
}