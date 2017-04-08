#include "Bullet.h"
#include "Globals.h"

//--Functions for the bullet

Bullet::Bullet(float x, float y)
{
	GameObject::Init(x, y, 0.050, 0.050, 1, 0, 0.25, 0.25);

	SetID(BULLET);


}

Bullet::Bullet(float x, float y, int ID)
{
	GameObject::Init(x, y, 0.052, 0.052, 1, 0, 0.25, 0.25);

	SetID(ID);
}

Bullet::Bullet(float x, float y, int ID, float x_dir, float y_dir)
{
	//GameObject::Init(x, y, 0.052, 0.052, x_dir, y_dir, 0.25, 0.25); //UNI COMPILE
	GameObject::Init(x, y, 0.00152, 0.00152, x_dir, y_dir, 0.25, 0.25); //HOME COMPILE

	SetID(ID);
}

void Bullet::Destroy()
{
	GameObject::Destroy();
}

void Bullet::Update()
{
	GameObject::Update();

	//--remove bullet if it hits edge of game screen 
	if(x > 5)
		SetAlive(false);
	else if(x < -5)
		SetAlive(false);

	if(y > 3.75)
		SetAlive(false);
	else if(y < -3.75)
		SetAlive(false);
}

void Bullet::Render(HDC deviceContext)
{
	GameObject::Render();

// array of vertex positions for a generic triangle (drawn with anticlockwise winding (google it) )
float vertexarray[] = { 
							-0.2f,	-0.2f,	0.0f,
							0.2f,	-0.2f,	0.0f,
							0.0f,	0.2f,	0.0f		};

// array of corresponding vertex colors, RGBA (note the american spelling of color)
float colorarray[] = { 
							1.0,	0.0,	0.0,	1.0,		// red
							1.0,	0.0,	0.0,	1.0,		// green
							1.0,	0.0,	0.0,	1.0		};	// blue

							// array of corresponding vertex colors, RGBA (note the american spelling of color)
float colorarray2[] = { 
							0.0,	1.0,	0.0,	1.0,		// red
							0.0,	1.0,	0.0,	1.0,		// green
							0.0,	1.0,	0.0,	1.0		};	// blue

// in this demo we always use the same piece of geometry (i.e. the triangle)...
		glVertexPointer(3, GL_FLOAT, 0, vertexarray);					// choose the array of vertex positions to use, 3 dimensional (XYZ), floats, 0 gap between elements
		if(Object_ID == BULLET2) //--draw second player's bullets
			glColorPointer(4, GL_FLOAT, 0, colorarray2);					// choose the array of vertex colors to use, 4 dimensional (RGBA), floats, 0 gap between elements
		else
			glColorPointer(4, GL_FLOAT, 0, colorarray);


			glLoadIdentity();											// start with the identity matrix (i.e. no transformation)
			glTranslatef(x,y,-10.0);	// then move across and back

			// then rotate to the current player's rotation around z axis
			glRotatef(rotation, 0.0, 0.0, 1.0);

			// then draw the 3 vertices from the array to make the generic triangle.
			// because we are using matrices the same triangle can be rotated differently for each player!
			glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Bullet::Collided(int objectID)
{
	if(objectID == BULLET || objectID == BULLET2)
		SetAlive(false);
}