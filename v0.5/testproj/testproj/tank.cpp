#include "tank.h"
#include "Globals.h"

//--functions for the main tank

Tank::Tank(int ID)
{
	Tank::x = x;
	Tank::y = y;
	Tank::Object_ID = ID;
	//Tank::x_speed = 0.025f; //UNI COMPILE
	Tank::x_speed = 0.0005f; //HOME COMPILE
	//Tank::y_speed = -0.025f; //UNI COMPILE
	Tank::y_speed = 0.0005f; //HOME COMPILE
	Tank::accelerationX = 0;
	Tank::accelerationDelayX = 0;
	Tank::accelerationY = 0;
	Tank::accelerationDelayY = 0;
	Tank::gunDelay = 0;
	Tank::y_bound = 0.5;
	Tank::x_bound = 0.5;
	Tank::shields = false;
	Tank::score = 0;
	Tank::Powerup = NONE;
}

void Tank::Destroy()
{
	//Tank::Destroy();
}

void Tank::Init(float x,float y)
{
	GameObject::Init(x,y,x_speed,y_speed,x_dir,y_dir,x_bound,y_bound);

	SetAlive(true);

	rotation = 0;
	Powerup = NONE;
}

void Tank::Update()
{
	GameObject::Update();
	if(x > 5)
		x = -5;
	else if(x < -5)
		x = 5;

	if(y > 3.75)
		y = -3.75;
	else if(y < -3.75)
		y = 3.75;
}

void Tank::Render(HDC deviceContext)
{
	GameObject::Render();

// array of vertex positions for a generic triangle (drawn with anticlockwise winding (google it) )
float vertexarrayshields[] = { 
							-0.6f,	-0.6f,	0.0f,
							0.6f,	-0.6f,	0.0f,
							-0.6f,	0.6f,	0.0f,
						};
float vertexarrayshields2[] = { 
							-0.6f,	0.6f,	0.0f,
							0.6f,	-0.6f,	0.0f,
							0.6f,	0.6f,	0.0f,
						};

// array of corresponding vertex colors, RGBA (note the american spelling of color)
float colorarrayshields[] = { 
							1.0,	1.0,	1.0,	1.0,		// red
							1.0,	1.0,	1.0,	1.0,		// green
							1.0,	1.0,	1.0,	1.0		};	// blue

// array of corresponding vertex colors, RGBA (note the american spelling of color)
float colorarrayspeed[] = { 
							1.0,	0.0,	0.0,	1.0,		// red
							1.0,	0.0,	0.0,	1.0,		// green
							1.0,	0.0,	0.0,	1.0		};	// blue

	if(shields == true || Powerup != NONE) { //--draws aura if shields are on, or powerup is in effect

		//--Load first triangle vertex array
		glVertexPointer(3, GL_FLOAT, 0, vertexarrayshields);
		if(Powerup == SPEEDUP)
			//--Loads red speedup powerup aura color array if the tank is under speedup effect
		glColorPointer(4, GL_FLOAT, 0, colorarrayspeed);
		else
			//--Loads white shields/invincibility aura color array otherwise
		glColorPointer(4, GL_FLOAT, 0, colorarrayshields);

		//--Drawing the first triangle
			glLoadIdentity();
			glTranslatef(x,y,-10.0);	// then move across and back


			glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//--Drawing the second triangle
			//--Load second triangle vertex array
			glVertexPointer(3, GL_FLOAT, 0, vertexarrayshields2);					// choose the array of vertex positions to use, 3 dimensional (XYZ), floats, 0 gap between elements
			glLoadIdentity();											// start with the identity matrix (i.e. no transformation)
			glTranslatef(x,y,-10.0);	// then move across and back

			// then draw the 3 vertices from the array to make the generic triangle.
			// because we are using matrices the same triangle can be rotated differently for each player!
			glDrawArrays(GL_TRIANGLES, 0, 3);
	}

		// array of vertex positions for a generic triangle (drawn with anticlockwise winding (google it) )
float vertexarray[] = { 
							-0.5f,	-0.5f,	0.0f,
							0.5f,	-0.5f,	0.0f,
							-0.5f,	0.5f,	0.0f,
						};
float vertexarray2[] = { 
							-0.5f,	0.5f,	0.0f,
							0.5f,	-0.5f,	0.0f,
							0.5f,	0.5f,	0.0f,
						};

// array of corresponding vertex colors, RGBA (note the american spelling of color)
float colorarray[] = { 
							0.0,	1.0,	0.0,	1.0,		// red
							0.0,	1.0,	0.0,	1.0,		// green
							0.0,	1.0,	0.0,	1.0		};	// blue

// array of corresponding vertex colors, RGBA (note the american spelling of color)
float colorarray2[] = { 
							0.0,	0.0,	1.0,	1.0,		// red
							0.0,	0.0,	1.0,	1.0,		// green
							0.0,	0.0,	1.0,	1.0		};	// blue

	//--Load first triangle vertex array
		glVertexPointer(3, GL_FLOAT, 0, vertexarray);					
		if(Object_ID == PLAYER2) 
			//--Loads second player's colour array
		glColorPointer(4, GL_FLOAT, 0, colorarray2);					
		else
			//--Loads first player's colour array
		glColorPointer(4, GL_FLOAT, 0, colorarray);	

			//--Drawing the first triangle
			glLoadIdentity();											// start with the identity matrix (i.e. no transformation)
			glTranslatef(x,y,-10.0);	// then move across and back

			// then draw the 3 vertices from the array to make the generic triangle.
			// because we are using matrices the same triangle can be rotated differently for each player!
			glDrawArrays(GL_TRIANGLES, 0, 3);

			//--Loading the second triangle vertex array and drawing
			glVertexPointer(3, GL_FLOAT, 0, vertexarray2);					// choose the array of vertex positions to use, 3 dimensional (XYZ), floats, 0 gap between elements
			glLoadIdentity();											// start with the identity matrix (i.e. no transformation)
			glTranslatef(x,y,-10.0);	// then move across and back

			// then draw the 3 vertices from the array to make the generic triangle.
			// because we are using matrices the same triangle can be rotated differently for each player!
			glDrawArrays(GL_TRIANGLES, 0, 3);

float vertexarraygun[] = { 
							-0.3f,	-0.3f,	0.0f,
							0.3f,	-0.3f,	0.0f,
							0.0f,	0.3f,	0.0f,
						};

// array of corresponding vertex colors, RGBA (note the american spelling of color)
float colorarraygun[] = { 
							1.0,	0.0,	0.0,	1.0,		// red
							1.0,	0.0,	0.0,	1.0,		// green
							0.0,	0.0,	0.0,	1.0		};	// blue
float colorarraygun2[] = { 
							0.0,	1.0,	0.0,	1.0,		// red
							0.0,	1.0,	0.0,	1.0,		// green
							0.0,	0.0,	0.0,	1.0		};	// blue

		//--Loading gun vertex array
		glVertexPointer(3, GL_FLOAT, 0, vertexarraygun);
		//--Loading gun color array for second player
		if(Object_ID == PLAYER2)
		glColorPointer(4, GL_FLOAT, 0, colorarraygun2);
		else
			//--Loading gun color array for first player otherwise
		glColorPointer(4, GL_FLOAT, 0, colorarraygun);
			glLoadIdentity();										
			glTranslatef(x,y,-10.0);	// then move across and back

			// then rotate to the current player's rotation around z axis
			glRotatef(rotation, 0.0, 0.0, 1.0);

			// then draw the 3 vertices from the array to make the generic triangle.
			// because we are using matrices the same triangle can be rotated differently for each player!
			glDrawArrays(GL_TRIANGLES, 0, 3);
}
			 

void Tank::MoveUp()
{
	increaseAccelerationY();
	if(Powerup == SPEEDUP)
		y_dir = 2.0;
	else
	y_dir = 1.0;
}
void Tank::MoveDown()
{
	increaseAccelerationY();
	if(Powerup == SPEEDUP)
		y_dir = -2.0;
	else
	y_dir = -1.0;
}
void Tank::MoveLeft()
{
	increaseAccelerationX();
	if(Powerup == SPEEDUP)
	x_dir = -2.0;
	else
	x_dir = -1.0;
}
void Tank::MoveRight()
{
	increaseAccelerationX();
	if(Powerup == SPEEDUP)
	x_dir = 2.0;
	else
	x_dir = 1.0;
}
void Tank::ResetMovementX()
{
	x_dir = 0.0;
	accelerationX = 0;
}
void Tank::ResetMovementY()
{
	y_dir = 0.0;
	accelerationY = 0;
}

void Tank::Collided(int objectID)
{
	//if(objectID == ENEMY) {
	//	SetHealth(health - 20);
	//lives--;
	//}
}

