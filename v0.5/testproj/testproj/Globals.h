#pragma once

//--Global libraries and variables to be loaded for each object/script

#include <windows.h>	// need to be able to create windows etc
#include <stdio.h>			//-- Header File For Standard Input/Output
#include <gl/gl.h>		// opengl header file
#include <gl/glu.h>		// glu header file - glu helps us set up the perspective projection


// some basic numbers to hold the position and size of the window
#define WIDTH		800
#define HEIGHT		600
#define TOPLEFTX	50
#define TOPLEFTY	50

// some player constants
#define ROTATION_SPEED	4.6f
#define NUM_PLAYERS		2


//#define BULLET_DELAY_MAX 50 //for uni compile
#define BULLET_DELAY_MAX 1999 //For home compile

//#define POWERUP_DELAY_MAX 199  //for uni compile
#define POWERUP_DELAY_MAX 19999 //For home compile

//--define some game screen borders
#define XLEFTBORDER -5f
#define XRIGHTBORDER 5f
#define YTOPBORDER 3.75f
#define YBOTTOMBORDER -3.75f

//--Acceleration parameters
#define ACCELERATION 0.0001f //for home compile
#define ACCELERATION_MAX 0.001f //for home compile
#define ACCELERATION_DELAY_MAX 500 //for home compile
//#define ACCELERATION_DELAY_MAX 10 //for uni compile

// some constants to help normalise the frame rate between different machines:

// IDEAL_FPS is a frame rate we can REASONABLY require from the user's PC, but is also high enough to provide smooth animation.
#define IDEAL_FPS		30

// IDEAL_DELTA is how long each frame would take in milliseconds if the game was running at the ideal frame rate.
#define IDEAL_DELTA		(1000 / IDEAL_FPS)

// later in the code, all animation updates will be normalised so the game will appear to run at the IDEAL_FPS, regardless of actual frame rate.
// this is why the IDEAL_FPS needs to be decent enough for smooth animation.
// *BUT* the downside to this is that if the PC cannot achieve a frame rate of IDEAL_FPS, the animation may appear jerky,
// because it will be updated by larger amounts in order to appear to be running at correct speed. 
// this is why the IDEAL_FPS also has to be easily-achievable by the user's pc, to minimise the chance of jerkiness.

// include the opengl and glu libraries
#pragma comment(lib, "opengl32.lib")	
#pragma comment(lib, "glu32.lib")

//--enumerate a list of Object IDs - For use in colors and collision detection
enum ID{PLAYER, PLAYER2, POWERUPI, POWERUPS, BULLET, BULLET2, BORDER, MISC};

//--enumerate a list of powerups - For use
enum POWERUP{NONE, SPEEDUP, INVINCIBLE};

// this class holds all of the information about a game state
class gameInfo
{
	public:
		gameInfo();			// the constructor
		~gameInfo();		// the destructor

		
		// variables required for the frame delta (DWORD is basically a long integer)
		DWORD frameStartTime;	// what the system time (in millisecs) was at the start of the last frame
		DWORD frameEndTime;		// what the system time (in millisecs) was at the end of the last frame 
		DWORD frameDelta;		// difference between the two (in millisecs), i.e. how long the last frame took		
};

