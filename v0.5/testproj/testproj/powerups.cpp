#include "powerups.h"
#include "Globals.h"

//--Functions for the powerups

PowerupS::PowerupS(float x, float y, int ID)
{
	//--Initialise powerup's location and boundary size
	GameObject::Init(x, y, 0, 0, 0, 0, 0.25, 0.25);

	//--Set object ID (Which powerup the object will be)
	SetID(ID);
}

void PowerupS::Destroy()
{
	GameObject::Destroy();
}

void PowerupS::Update()
{
	GameObject::Update();

	//--Remove powerup in case it spawns outside of game borders 
	if(x > 5)
		SetAlive(false);
	else if(x < -5)
		SetAlive(false);

	if(y > 3.75)
		SetAlive(false);
	else if(y < -3.75)
		SetAlive(false);
}

void PowerupS::Render(HDC deviceContext)
{
	GameObject::Render();

float vertexarray[] = { 
							-0.2f,	-0.2f,	0.0f,
							0.2f,	-0.2f,	0.0f,
							0.0f,	0.2f,	0.0f		};

//--Colour array for the speed powerup (Red)
float colorarray[] = { 
							0.0,	0.0,	1.0,	1.0,		// red
							0.0,	0.0,	1.0,	1.0,		// green
							0.0,	0.0,	1.0,	1.0		};	// blue

//--Colour array for the invincibility powerup (White)
float colorarray2[] = { 
							1.0,	1.0,	1.0,	1.0,		// red
							1.0,	1.0,	1.0,	1.0,		// green
							1.0,	1.0,	1.0,	1.0		};	// blue

	//--Load the same vertex array
		glVertexPointer(3, GL_FLOAT, 0, vertexarray);
		//--Load speedup colour array if the object is a speedup
		if(Object_ID == POWERUPS)
			glColorPointer(4, GL_FLOAT, 0, colorarray);					
		else //--Load invincibility colour array otherwise
			glColorPointer(4, GL_FLOAT, 0, colorarray2);

			glLoadIdentity();			
			glTranslatef(x,y,-10.0);	//--Move object to it's in-game position

			//--Draw triangle on screen!
			glDrawArrays(GL_TRIANGLES, 0, 3);
}

void PowerupS::Collided(int objectID)
{
	SetAlive(false);
}