#ifndef GYROGAME_H_
#define GYROGAME_H_


#include "Arduino.h"
#include "ArduinoSTL.h"
#include <Adafruit_GFX.h>
#include <string.h>


/////////////////////////////////////////////////
// HELPER FUNCTIONS
/////////////////////////////////////////////////

constexpr inline unsigned short rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
};



constexpr unsigned char
	GYROSCOPE_PIN_X	= 13,		// Pin numbers for the gyroscope sensor
	GYROSCOPE_PIN_Y	= 14,
	GYROSCOPE_PIN_Z	= 15;

constexpr unsigned short int
	COLOR_BLACK		= 0x0000,	// black for status bar background
	COLOR_YELLOW	= rgb(255, 255, 0),	// yellow and...
	COLOR_WHITE		= rgb(255, 255, 255),	// ...white for text output
	COLOR_BLUE		= rgb(0, 0, 100),	// a dark blue for background
	COLOR_GREEN		= rgb(0, 255, 0),	// intense green for player
	COLOR_RED		= rgb(255, 0, 0);	// intense red for enemies

constexpr unsigned char
	DIR_NODIRECTION	= 55,		// directions for movement
	DIR_UP			= 1,
	DIR_DOWN		= 2,
	DIR_LEFT		= 3,
	DIR_RIGHT		= 4;


class Gamefield;
class Player;
class Enemy;
class Gyroscope;
class Coutdown;
class StatusBar;
class EnemyField;


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

typedef std::vector<Enemy> EnemyVector;		// vector to hold enemy-objects


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
		void setWidth(short int w);
		void setColor(unsigned short c);
		void setPosition(int x, int y);		// set position directly
		void setSpeed(SpeedT s);			// set player's speed
		void setName();						// set the player name
		void move();						// set position by direction
		void draw();
		short int getX();
		short int getY();
		short int getWidth();
		void addScore(short int value);
		short int getScore();
		bool scoreChanged();
	private:
		Gamefield *gamefield;				// join a game field
		Adafruit_GFX *screen;				// display to draw to
		PositionT pos;
		PositionT oldPos;
		SpeedT speed;
		unsigned short int color = COLOR_GREEN;
		short int width = 20;
		short int score = 0;
		short int scoreOld = 0;
		char name[4] = {'M','J','K','\0'};	//store three letters plus "\0"
};

class Enemy {
	public:
		Enemy(Gamefield *g, Adafruit_GFX *s);
		void setPosition(int x, int y); // set position directly
		void randomPosition();			// set random position and random direction
		void setDirection(unsigned char x, unsigned char y);
		void setSpeed(unsigned char s);
		void setWidth(short int w);		// set enemy size
		void move();					// set position by direction
		void draw();
		void unDraw();					// erase actual position (e.g. before deletion)
		short int getX();
		short int getY();
		short int getWidth();			// get enemy size
		bool isFriendly();				// can be eaten
		void makeFriendly();
	private:
		Gamefield *gamefield;			// joined game field
		Adafruit_GFX *screen;			// display to draw to
		PositionT pos;
		PositionT oldPos;
		DirectionT dir;
		short int width = 6;
		unsigned char speed = 1;		// enemies speed is the same on every axis
		bool friendly = false; 			// the friendly ones can be catched for points
};

class Gyroscope {
	public:
		Gyroscope();
		~Gyroscope();
		void read();		// get state
		void calibrate();
		SpeedT getSpeed();
	private:
		int x = 0;			// current values
		int y = 0;
		int z = 0;
		int calibX = 0;		// values saved for calibration: I implemented this because players...
		int calibY = 0; 	// ...don't have to hold the game exactly in balance...
		int calibZ = 0; 	// ...at the start or the game.
		SpeedT speed;
};

class Countdown {
	public:
		Countdown();
		~Countdown();
		void init(unsigned int s);
		void start();					// start or reset timer
		void togglePause();				// TP DO: pause (true) or continue (false)
		void update();					// update timer's seconds left
		bool expired();					// checks if timer has expired
		unsigned int getTimeLeft();		// seconds left
		unsigned int getTimeLeftOld();	// to check if something changed after last update()
	private:
		unsigned long pauseTimeStarted;	// (ms) this is 'millis()' when timer is was paused
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
		void drawAll();
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

class EnemyField {
	public:
		EnemyField(Player *p, Gamefield *g, Adafruit_GFX *s, unsigned short int nE);
		~EnemyField();
		void randomPosition(Enemy *e);		// set randomized position and direction for an enemy
		void draw();						// erase old position, draw actual position
		void move();
		void addEnemy(bool f);				// add enemy to vector (f=friendly)
		void removeEnemy(unsigned char n);	// remove enemy from vector
		unsigned char numEnemies();			// return number of enemies in vector
		void collisionTest();
	private:
		Player *player;
		Gamefield *gamefield;
		Adafruit_GFX *screen;
		unsigned short int numE;
		EnemyVector e;
};


#endif /* GYROGAME_H_ */
