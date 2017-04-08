

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// OpenGL without using GLUT - 2013 by Neil Dansey, Tim Dykes and Ian Cant, and using excerpts from here:
// http://bobobobo.wordpress.com/2008/02/11/opengl-in-a-proper-windows-app-no-glut/
// Feel free to adapt this for what you need, but please leave these comments in.

//--2014 "Tank Game" work by Damon Wareham - all related comments denoted by '--'

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


#include <list>
//--Include other project files (LOCAL)
#include "Globals.h"
#include "GameObject.h"
#include "bullet.h"
#include "tank.h"
#include "powerups.h"

//--enumerate a list of keys for input
enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE, ENTER, HELPKEY, W, A, S, D, Q, E, DEL, PGDWN, CTRL, TAB, BACKSPACE};
//--Create bool array for keys to hold "on" or "off" state
extern bool keys[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

//--Create iterator lists, used for handling collision detection, updating and rendering
std::list<GameObject *> objects;
std::list<GameObject *>::iterator it01;
std::list<GameObject *>::iterator it02;

//--OpenGL libraries and screen size definitions moved to Globals.h


// function prototypes:

// these were already here as part of the windows application template...
LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );	
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow );

//--Prototype main render and update functions
void draw(HDC deviceContext, gameInfo* currentGame);
void update(HDC deviceContext, gameInfo* currentGame);


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// In a C++ Windows app, the starting point is WinMain() rather than _tmain() or main().
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{


	// some variables need for our game...
    MSG msg;								// this will be used to store messages from the operating system
	bool needToQuit = false;				// whether or not the player has quit the app
	gameInfo* currentGame = new gameInfo;	// this creates our game instance, and calls it currentGame



	// this bit creates a window class, basically a template for the window we will make later, so we can do more windows the same.
    WNDCLASS myWindowClass;
    myWindowClass.cbClsExtra = 0;											// no idea
    myWindowClass.cbWndExtra = 0;											// no idea
    myWindowClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );	// background fill black
    myWindowClass.hCursor = LoadCursor( NULL, IDC_ARROW );					// arrow cursor       
    myWindowClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );				// type of icon to use (default app icon)
    myWindowClass.hInstance = hInstance;									// window instance name (given by the OS when the window is created)   
    myWindowClass.lpfnWndProc = WndProc;									// window callback function - this is our function below that is called whenever something happens
    myWindowClass.lpszClassName = TEXT("my window class name");				// our new window class name
    myWindowClass.lpszMenuName = 0;											// window menu name (0 = default menu?) 
    myWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// redraw if the window is resized horizontally or vertically, allow different context for each window instance

    // Register that class with the Windows OS..
	RegisterClass(&myWindowClass);
    
   
	// create a rect structure to hold the dimensions of our window
    RECT rect;
    SetRect( &rect, TOPLEFTX,				// the top-left corner x-coordinate
                    TOPLEFTY,				// the top-left corner y-coordinate
                    TOPLEFTX + WIDTH,		// far right
                    TOPLEFTY + HEIGHT );	// far left
    

    
    // adjust the window, no idea why.
    AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );

  
    
    // call CreateWindow to create the window
   HWND myWindow = CreateWindow(TEXT("my window class name"),		// window class to use - in this case the one we created a minute ago
                          TEXT("Tank game! Damon Wareham 2014"),					// window title
                          WS_OVERLAPPEDWINDOW,						// ??
                          TOPLEFTX, TOPLEFTY,						// x, y
                          TOPLEFTX + WIDTH, TOPLEFTY + HEIGHT,		// width and height
                          NULL, NULL,								// ??
                          hInstance, NULL);							// ??

    
   // check to see that the window created okay
    if( myWindow == NULL )
    {
        FatalAppExit( NULL, TEXT("CreateWindow() failed!") );
    }

    // if so, show it
    ShowWindow( myWindow, iCmdShow );
   

    // get a device context from the window
    HDC myDeviceContext = GetDC( myWindow );


	// we create a pixel format descriptor, to describe our desired pixel format. 
	// we set all of the fields to 0 before we do anything else
	// this is because PIXELFORMATDESCRIPTOR has loads of fields that we won't use
    PIXELFORMATDESCRIPTOR myPfd = { 0 };
 

    // now set only the fields of the pfd we care about:
    myPfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );		// size of the pfd in memory
    myPfd.nVersion = 1;									// always 1

    myPfd.dwFlags = PFD_SUPPORT_OPENGL |				// OpenGL support - not DirectDraw
					PFD_DOUBLEBUFFER   |				// double buffering support
					PFD_DRAW_TO_WINDOW;					// draw to the app window, not to a bitmap image

    myPfd.iPixelType = PFD_TYPE_RGBA ;					// red, green, blue, alpha for each pixel
    myPfd.cColorBits = 24;								// 24 bit == 8 bits for red, 8 for green, 8 for blue.
														// This count of color bits EXCLUDES alpha.

    myPfd.cDepthBits = 32;								// 32 bits to measure pixel depth.



	// now we need to choose the closest pixel format to the one we wanted...	
    int chosenPixelFormat = ChoosePixelFormat( myDeviceContext, &myPfd );
 
	// if windows didnt have a suitable format, 0 would have been returned...
    if( chosenPixelFormat == 0 )
    {
        FatalAppExit( NULL, TEXT("ChoosePixelFormat() failed!") );
    }

	// if we get this far it means we've got a valid pixel format
	// so now we need to set the device context up with that format...
    int result = SetPixelFormat( myDeviceContext, chosenPixelFormat, &myPfd );

	// if it failed...
    if (result == NULL)
    {
        FatalAppExit( NULL, TEXT("SetPixelFormat() failed!") );
    }

  
    // we now need to set up a render context (for opengl) that is compatible with the device context (from windows)...
    HGLRC myRenderContext = wglCreateContext( myDeviceContext );

	// then we connect the two together
    wglMakeCurrent( myDeviceContext, myRenderContext );


    // opengl display setup
	glMatrixMode(GL_PROJECTION);	// select the projection matrix, i.e. the one that controls the "camera"
    glLoadIdentity();				// reset it
    gluPerspective(45.0,(float)WIDTH/(float)HEIGHT, 1, 1000);	// set up fov, and near / far clipping planes
	glViewport(0, 0, WIDTH, HEIGHT);							// make the viewport cover the whole window
	glClearColor( 0, 0, 0, 0 );								// set the colour used for clearing the screen



	//--initialising and placing first player's tank
	Tank *tank1 = new Tank(PLAYER);
	objects.push_back(tank1);
	tank1->Init(4,3);
	
	//--initialising and placing second player's tank
	Tank *tank2 = new Tank(PLAYER2);
	objects.push_back(tank2);
	tank2->Init(-4,-3);


	// as long as the player hasnt exited the app: 
    while(needToQuit == false)
    {

		// this is the start of the main game loop, so we need to record the time...
		currentGame->frameStartTime = GetTickCount();
		
		// we need to listen out for OS messages.
		// if there is a windows message to process...
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
			
			// if the message was a "quit" message...
			if( msg.message == WM_QUIT )
			{
				needToQuit = true;	// we want to quit asap
			}
			// otherwise, if it was a "key pressed" message...
			else if (msg.message == WM_KEYDOWN)
			{
				switch (msg.wParam)
				{
					//--Switch case statement, checks for any case which is true
					case VK_RIGHT:
						keys[RIGHT] = true; //--Sets the right array key to true, used later in code
						break;

					case VK_LEFT:
						keys[LEFT] = true;
						break;

					case VK_UP:
						keys[UP] = true;
						break;

					case VK_DOWN:
						keys[DOWN] = true;
						break;

					case 0x57: //--W key
						keys[W] = true;
						break;

					case 0x41: //--A key
						keys[A] = true;
						break;

					case 0x53: //--S key
						keys[S] = true;
						break;

					case 0x44: //--D key
						keys[D] = true;
						break;

					case VK_DELETE:
						keys[DEL] = true;
						break;

					case VK_NEXT: //--Page Down Key
						keys[PGDWN] = true;
						break;

					case 0x51: //--Q key
						keys[Q] = true;
						break;

					case 0x45: //--E key
						keys[E] = true;
						break;

					case VK_CONTROL: //--CTRL key
						keys[CTRL] = true;
						break;

					case VK_SPACE:
						keys[SPACE] = true;
						break;

					case VK_ESCAPE:
						needToQuit = true;	//--we want to quit asap
						break;

					case VK_TAB:
						keys[TAB] = true;
						break;

					case VK_BACK:
						keys[BACKSPACE] = true;
						break;

					default:
						//nothing
						break;

				}
			}
			// otherwise, if they let go of a key (you get the idea)
			else if (msg.message == WM_KEYUP)
			{
				switch (msg.wParam)
				{
					case VK_RIGHT:
						keys[RIGHT] = false;
						break;


					case VK_LEFT:
						keys[LEFT] = false;	
						break;

					case VK_UP:
						keys[UP] = false;
						break;

					case VK_DOWN:
						keys[DOWN] = false;
						break;

					case 0x57: //--W key
						keys[W] = false;
						break;

					case 0x41: //--A key
						keys[A] = false;
						break;

					case 0x53: //--S key
						keys[S] = false;
						break;

					case 0x44: //--D key
						keys[D] = false;
						break;

					case VK_SPACE:
						keys[SPACE] = false;
						break;

					case VK_DELETE:
						keys[DEL] = false;
						break;

					case VK_NEXT: //--Page Down Key
						keys[PGDWN] = false;
						break;

					case VK_CONTROL: //CTRL key
						keys[CTRL] = false;
						break;

					case 0x51: //--Q key
						keys[Q] = false;
						break;

					case 0x45: //--E key
						keys[E] = false;
						break;

					case VK_TAB:
						keys[TAB] = false;
						break;

					case VK_BACK:
						keys[BACKSPACE] = false;
						break;

					default:
						//nothing
						break;

				}
			}
			
			// if it was any other type of message (i.e. one we don't care about), process it as normal
            else 
            {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
            }

        }

		float deltaAdjustment = (float)currentGame->frameDelta / IDEAL_DELTA;

		//--Counts down before guns can fire again
		if((tank1->GetGunDelay()) > 0)
                       tank1->SetGunDelay((tank1->GetGunDelay()) - 1);
		if((tank2->GetGunDelay()) > 0)
                       tank2->SetGunDelay((tank2->GetGunDelay()) - 1);

		//--Counts down how long a player's powerup is in effect
		if((tank1->GetPowerupDelay()) > 0)
                       tank1->SetPowerupDelay((tank1->GetPowerupDelay()) - 1);
		else tank1->SetPowerup(NONE);
		if((tank2->GetPowerupDelay()) > 0)
                       tank2->SetPowerupDelay((tank2->GetPowerupDelay()) - 1);
		else tank2->SetPowerup(NONE);
				


		//--Processes controls for first player

		if(!tank1->CheckShields()) { //--ONLY ABLE TO MOVE, ROTATE AND SHOOT WITH SHIELDS OFF

				//--Moving up/down
				if(keys[UP])
					tank1->MoveUp();
				else if(keys[DOWN])
					tank1->MoveDown();
				else {
				tank1->ResetMovementY();
				tank1->decreaseAccelerationY(); //--Resets speed when no movement keys are pressed
				}

				//--Moving left/right
				if(keys[LEFT])
					tank1->MoveLeft();
				else if(keys[RIGHT])
					tank1->MoveRight();
				else {
				tank1->ResetMovementX();
				tank1->decreaseAccelerationX(); //--Resets speed when no movement keys are pressed
				}

				//--Rotating turret
				if(keys[DEL]) {
					tank1->rotation -= (ROTATION_SPEED * deltaAdjustment);
					if(tank1->GetRotation() < 0)
						tank1->SetRotation(360);
				}
				else if(keys[PGDWN]) {
					tank1->rotation += (ROTATION_SPEED * deltaAdjustment);
					if(tank1->GetRotation() > 360)
						tank1->SetRotation(0);
				}

				//--Firing gun, based on turret's rotation
				if((keys[SPACE]) && (tank1->GetGunDelay() == 0)) {
					tank1->SetGunDelay(BULLET_DELAY_MAX);
					if(tank1->GetRotation() > 337.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, 0, 1);
					objects.push_back(bullet);
					}
					else if(tank1->GetRotation() > 292.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, 1, 1);
					objects.push_back(bullet);
					}
					else if(tank1->GetRotation() > 247.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, 1, 0);
					objects.push_back(bullet);
					}
					else if(tank1->GetRotation() > 202.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, 1, -1);
					objects.push_back(bullet);
					}
					else if(tank1->GetRotation() > 157.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, 0, -1);
					objects.push_back(bullet);
					}
					else if(tank1->GetRotation() > 112.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, -1, -1);
					objects.push_back(bullet);
					}
					else if(tank1->GetRotation() > 67.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, -1, 0);
					objects.push_back(bullet);
					}
					else if(tank1->GetRotation() > 22.5) {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, -1, 1);
					objects.push_back(bullet);
					}
					else {
					Bullet *bullet = new Bullet(tank1->GetX(), tank1->GetY(), BULLET, 0, 1);
					objects.push_back(bullet);
					}
				}

		}

				//--activate shields
				if(keys[BACKSPACE]) {
					tank1->SetShields(true);
					tank1->ResetMovementX();
					tank1->ResetMovementY();
				}
				else
					tank1->SetShields(false); //--Force shields off if button is not pressed



		//--Processes controls for second player

				if(!tank2->CheckShields()) { //--ONLY ABLE TO MOVE, ROTATE AND SHOOT WITH SHIELDS OFF

				//--Moving up/down
				if(keys[W])
					tank2->MoveUp();
				else if(keys[S])
					tank2->MoveDown();
				else tank2->ResetMovementY();

				//--Moving left/right
				if(keys[A])
					tank2->MoveLeft();
				else if(keys[D])
					tank2->MoveRight();
				else tank2->ResetMovementX();
				
				//--Rotating turret
				if(keys[Q]) {
					tank2->rotation -= (ROTATION_SPEED * deltaAdjustment);
					if(tank2->GetRotation() < 0)
						tank2->SetRotation(360);
				}
				else if(keys[E]) {
					tank2->rotation += (ROTATION_SPEED * deltaAdjustment);
					if(tank2->GetRotation() > 360)
						tank2->SetRotation(0);
				}

				//--Firing gun
				if((keys[CTRL]) && (tank2->GetGunDelay() == 0)) {
					tank2->SetGunDelay(BULLET_DELAY_MAX);
					if(tank2->GetRotation() > 337.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, 0, 1);
					objects.push_back(bullet);
					}
					else if(tank2->GetRotation() > 292.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, 0.5, 0.5);
					objects.push_back(bullet);
					}
					else if(tank2->GetRotation() > 247.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, 1, 0);
					objects.push_back(bullet);
					}
					else if(tank2->GetRotation() > 202.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, 0.5, -0.5);
					objects.push_back(bullet);
					}
					else if(tank2->GetRotation() > 157.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, 0, -1);
					objects.push_back(bullet);
					}
					else if(tank2->GetRotation() > 112.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, -0.5, -0.5);
					objects.push_back(bullet);
					}
					else if(tank2->GetRotation() > 67.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, -1, 0);
					objects.push_back(bullet);
					}
					else if(tank2->GetRotation() > 22.5) {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, -0.5, 0.5);
					objects.push_back(bullet);
					}
					else {
					Bullet *bullet = new Bullet(tank2->GetX(), tank2->GetY(), BULLET2, 0, 1);
					objects.push_back(bullet);
					}
				}

				}

				//--activate shields
				if(keys[TAB]) {
					tank2->SetShields(true);
					tank2->ResetMovementX();
					tank2->ResetMovementY();
				}
				else
					tank2->SetShields(false);

				//if(rand() % 2500 == 0) //UNI COMPILE
				if(rand() % 99999999 == 0) //HOME COMPILE
					//--Spawns a Invincibility powerup at random
				{
					PowerupS *powerups = new PowerupS(5 - rand() % 10, 3.75 - rand() % 7, POWERUPI);
					objects.push_back(powerups);
				}

				//if(rand() % 2500 == 0) //UNI COMPILE
				if(rand() % 99999999 == 0) //HOME COMPILE
					//--Spawns a Speedup powerup at random
				{
					PowerupS *powerups = new PowerupS(5 - rand() % 10, 3.75 - rand() % 7, POWERUPS);
					objects.push_back(powerups);
				}

		
		// now update the gameplay...
		update(myDeviceContext, currentGame);

		//--Actions for any collisions

				//--Reset Tanks if they collide
				if( (tank1)->CheckCollisions(tank2))
				{
					tank1->SetX(((tank1->GetX()-tank2->GetX_Bound())));
					tank2->SetX(((tank2->GetX()-tank1->GetX_Bound())));
				}

				//--Collisions with player 1
				for(it01 = objects.begin(); it01 != objects.end(); ++it01)
				{
				if( (tank1)->CheckCollisions(*it01))
				{
					if((*it01)->CheckObjectID(BULLET2)) { //--If hit by other player's bullet
						if(!tank1->CheckShields()) {
							if(tank1->CheckPowerup() != INVINCIBLE) {
					tank1->Init(4,3);
					tank2->AddPoint();
							}
						}
					(*it01)->SetAlive(false);
					}
					else if((*it01)->CheckObjectID(POWERUPI)) { //--If hit by invincibility powerup
						if(tank1->CheckPowerup() == NONE) {
							tank1->SetPowerup(INVINCIBLE);
							tank1->SetPowerupDelay(POWERUP_DELAY_MAX);
							(*it01)->SetAlive(false);
						}
					}
					else if((*it01)->CheckObjectID(POWERUPS)) { //If hit by speedup powerup
						if(tank1->CheckPowerup() == NONE) {
							tank1->SetPowerup(SPEEDUP);
							tank1->SetPowerupDelay(POWERUP_DELAY_MAX);
							(*it01)->SetAlive(false);
						}
					}
				}

				//--Collisions with player 2
				if( (tank2)->CheckCollisions(*it01))
				{
					if((*it01)->CheckObjectID(BULLET)) {
						if(!tank2->CheckShields()) {
							if(tank2->CheckPowerup() != INVINCIBLE) {
					tank2->Init(-4,-3);
					tank1->AddPoint();
							}
						}
					(*it01)->SetAlive(false);
					}
					else if((*it01)->CheckObjectID(POWERUPI)) {
						if(tank2->CheckPowerup() == NONE) {
							tank2->SetPowerup(INVINCIBLE);
							tank2->SetPowerupDelay(POWERUP_DELAY_MAX);
							(*it01)->SetAlive(false);
						}
					}
					else if((*it01)->CheckObjectID(POWERUPS)) {
						if(tank2->CheckPowerup() == NONE) {
							tank2->SetPowerup(SPEEDUP);
							tank2->SetPowerupDelay(POWERUP_DELAY_MAX);
							(*it01)->SetAlive(false);
						}
					}
				}
			}

		// now draw the scene...
		draw(myDeviceContext, currentGame);
	
		 

		// this is the end of the main game loop, so we need to record the time again and then work out how long this frame took...
		currentGame->frameEndTime = GetTickCount();
		currentGame->frameDelta = (currentGame->frameEndTime - currentGame->frameStartTime);

		
		
		//--cull the dead - any objects set to "dead" will be removed and deleted from memory
			for(it01 = objects.begin(); it01 != objects.end(); )
			{
				if(! (*it01)->GetAlive())
				{
					delete (*it01);
					it01 = objects.erase(it01);
				}
				else
					it01++;
			}
    }

	// end of "while" loop
    // the next bit will therefore happen when the player quits the app,
	// because they are trapped in the game loop as long as (needToQuit == false).

	//--Clear all objects
	for(it01 = objects.begin(); it01 != objects.end(); )
	{
		(*it01)->Destroy();
		delete (*it01);
		it01 = objects.erase(it01);
	}

	// free up the memory used by the game state and set its pointer to NULL
    delete currentGame;
	currentGame = NULL;

    // UNmake our rendering context (make it 'uncurrent')
    wglMakeCurrent( NULL, NULL );

    // delete the rendering context, we no longer need it.
    wglDeleteContext( myRenderContext );

    // release our window's DC from the window
    ReleaseDC( myWindow, myDeviceContext );
  
	// end the program
    return msg.wParam;
}

//--Main draw function
void draw(HDC deviceContext, gameInfo* currentGame)   
{
    		
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		// clear the screen and the depth buffer

	glEnableClientState(GL_VERTEX_ARRAY);						// we need enable vertices to draw polygons (obviously)
	glEnableClientState(GL_COLOR_ARRAY);						// we want to use colors too
	
	glMatrixMode(GL_MODELVIEW);									// we are going to affect how models are drawn

	glPushMatrix();
	 //--all objects on list are rendered - all objects on iterator list have their render functions called
	for(it01 = objects.begin(); it01 != objects.end(); ++it01) {
					(*it01)->Render(deviceContext);
				}
		glPopMatrix();	
		glDisableClientState(GL_VERTEX_ARRAY);						// turn off vertex rendering again, just in case
		glDisableClientState(GL_COLOR_ARRAY);	
		SwapBuffers(deviceContext);	
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// this function is called when any events happen to our window
LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) 
{

    switch( message )
    {
		// if they exited the window...	
		case WM_DESTROY:
			// post a message "quit" message to the main windows loop
			PostQuitMessage( 0 ) ;
			return 0;
			break;
    }
 
	// must do this as a default case (i.e. if no other event was handled)...
    return DefWindowProc( hwnd, message, wparam, lparam );

}

// this sets up the default values that are common to all game instances
gameInfo::gameInfo()
{
	// since the updateGame function requires a delta time from the previous frame for its calculations, 
	// this will not work on the first frame of the game as there will be no delta recorded for it to work with.
	// so we will pretend a value was obtained, and that it was "ideal"...
	frameDelta = IDEAL_DELTA;

	// initialise the other frame delta variables...
	frameStartTime = 0;
	frameEndTime = 0;
}


gameInfo::~gameInfo()
{

}



//--Main update function
void update(HDC deviceContext, gameInfo* currentGame) {

	float deltaAdjustment = (float)currentGame->frameDelta / IDEAL_DELTA;
			//--all objects on list are updated - all objects on iterator list have their update functions called
				for(it01 = objects.begin(); it01 != objects.end(); ++it01)
						(*it01)->Update();

				//--Checking objects for collisions
				for(it01 = objects.begin(); it01 != objects.end(); ++it01)
				{
					if( ! (*it01)->Collidable() ) continue;

					for(it02 = it01; it02 != objects.end(); ++it02)
					{
						if( !(*it02)->Collidable() ) continue;
						if( (*it01)->GetID() == (*it02)->GetID()) continue;

						if( (*it01)->CheckCollisions( (*it02)))
						{
							(*it01)->Collided( (*it02)->GetID());
							(*it02)->Collided( (*it01)->GetID());
						}
					}
				}
}