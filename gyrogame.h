#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <string.h>


#define GYROSCOPE_PIN_X	13		// Pin numbers for the gyroscope sensor
#define GYROSCOPE_PIN_Y	14
#define GYROSCOPE_PIN_Z	15

#define SCREEN_WIDTH	470		// Change for other displays
#define SCREEN_HEIGHT	290
#define MAX_ENEMIES		30		// max number of enemies (to keep in memory)

#define COLOR_BLACK		0x0000	// black for status bar background
#define COLOR_YELLOW	0xFFE0	// yellow and...
#define COLOR_WHITE		0xFFE0	// ...white for text output
#define COLOR_BLUE		0x0036	// a dark blue for background
#define COLOR_GREEN		0x07E0	// intense green for player
#define COLOR_RED		0xF800	// intense red for enemies

#define DIR_NODIRECTION	55		// directions for movement
#define DIR_UP			1
#define DIR_DOWN		2
#define DIR_LEFT		3
#define DIR_RIGHT		4



struct PositionT {
	short int x = 0;
	short int y = 0;
};

struct DirectionT {
	unsigned char x;	// left/right
	unsigned char y;	// up/down
};

struct SpeedT {
	short int x;	// left/right
	short int y;	// up/down
};



/////////////////////////////////////////////////
// HELPER FUNCTIONS
/////////////////////////////////////////////////

unsigned short rgb(uint8_t r, uint8_t g, uint8_t b);	// covert RGB to RGGB
void centerText(Adafruit_GFX *scr, char txt[], short int l, short int t, short int w, short int h, unsigned short color, unsigned short size);



/////////////////////////////////////////////////
// CLASSES
/////////////////////////////////////////////////

class Gamefield {
	public:
		Gamefield(Adafruit_GFX *s);
		void init(short int left, short int top, short int width, short int height, unsigned short int color, unsigned short int borderColor);
		short int getLeft();
		short int getTop();
		short int getWidth();
		short int getHeight();
		unsigned short int getColor();
		void draw();
	private:
		short int top = 20;
		short int left = 0;
		short int height = 300;
		short int width = 480;
		Adafruit_GFX *screen;
		unsigned short int color = COLOR_BLUE;		// background of the game field
		unsigned short int borderColor = COLOR_WHITE;	// border color
};

class Player {
	public:
		Player(Gamefield *g, Adafruit_GFX *s);
		void init(short int x, short int y, unsigned short col);
		void setRadius(short int r);
		void setColor(unsigned short c);
		void setPosition(int x, int y);		// set position directly
		void setSpeed(SpeedT s);			// set player's speed
		void setName();						// set the player name
		void move();						// set position by direction
		void draw();
		short int getX();
		short int getY();
		void addScore(short int value);
		unsigned int getScore();
	private:
		Gamefield *gamefield;				// join a game field
		Adafruit_GFX *screen;				// display to draw to
		PositionT pos;
		PositionT oldPos;
		SpeedT speed;
		unsigned short int color = COLOR_GREEN;
		short int radius = 15;
		short int score = 0;
		char name[4] = {'M','J','K','\0'};	//store three letters plus "\0"
};

class Enemy {
	public:
		Enemy(Gamefield *g, Adafruit_GFX *s);
		void init(short int x, short int y, unsigned short col);
		void setPosition(int x, int y); // set position directly
		void setDirection(unsigned char x, unsigned char y);
		void setSpeed(unsigned char s);
		void setRadius(short int r);
		void move();					// set position by direction
		void draw();
		short int getX();
		short int getY();
		short int getRadius();
	private:
		Gamefield *gamefield;			// joined game field
		Adafruit_GFX *screen;			// display to draw to
		PositionT pos;
		PositionT oldPos;
		DirectionT dir;
		unsigned short int color = COLOR_RED;
		unsigned char radius = 3;
		unsigned char speed = 2;		// enemies speed is the same on every axis
};

class Gyroscope {
	public:
		Gyroscope();
		~Gyroscope();
		void read();
		void calibrate();
		SpeedT getSpeed();
	private:
		int x = 0;		// current values
		int y = 0;
		int z = 0;
		int calibX = 0;	// values saved for calibration: I implemented this because players...
		int calibY = 0; // ...don't have to hold the game exactly in balance...
		int calibZ = 0; // ...at the start or the game.
		SpeedT speed;
};

class Countdown {
	public:
		Countdown();
		~Countdown();
		void init(unsigned int s);
		void start();		// start or reset timer
		void togglePause();	// pause (true) or continue (false)
		void update();		// update timer's seconds left
		bool expired();		// checks if timer has expired
		unsigned int getTimeLeft();
	private:
		unsigned long pauseTimeStarted;	// (ms) this is 'millis()' whend timer is was paused
		unsigned long timeStarted;		// (ms) this is 'millis()' when timer is set
		short int pauseTime = 0;		// (sec) timer for pause
		short int seconds = 60;			// (sec) number of total seconds (won't change during countdown)
		short int timeLeft = 60;		// (sec) seconds will count down
		short int timeLeftOld = 60;		// (sec) to see if the time has changed (i.e. for redraw)
		bool paused = false;			// is timer counting?
};

class StatusBar {
	public:
		StatusBar(Player *p, Countdown *c, Adafruit_GFX *s);
		void init(short int left, short int top, short int width, short int height, unsigned short int bkColor, unsigned short int borderColor);
		void draw();
	private:
		Player *player;					// use this player's score
		Countdown *countdown;			// use this countdown's time
		Adafruit_GFX *screen;
		short int left = 0;
		short int top = 0;
		short int width = 480;
		short int height = 20;
		unsigned short int bkColor = COLOR_BLUE;
		unsigned short int borderColor = COLOR_YELLOW;
		char caption[100] = "This is a test\0";
};


