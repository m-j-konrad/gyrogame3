// Do not remove the include below
#include "ArduinoSTL.h"
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Waveshare4InchTftShield.h>
#include "gyrogame.h"

#define error(s) sd.errorHalt(F(s))  // store error strings in flash to save RAM
Waveshare4InchTftShield Waveshield;
GFXcanvas8 canvas(480, 320);
Adafruit_GFX &tft = Waveshield;

typedef std::vector<Enemy> EnemyVector;

bool SDCard = false;	// is SD card available?
Gyroscope gyroscope;
Gamefield gamefield(&tft);
Player player(&gamefield, &tft);
EnemyVector enemies;	// enemies in a vector
Countdown timer;		// timer to count down
StatusBar statusBar(&player, &timer, &tft);	// status bar

void initDisplay();										// initialize TFT display (hardware, rotation)
void initSDCard();										// initialize SD card

void randomPositionEnemy(Enemy *e);						// set randomized position and direction for an enemy
void drawEnemies(EnemyVector *e);
void moveEnemies(EnemyVector *e);
void addEnemy(EnemyVector *e);							// add enemy to vector
void removeEnemy(EnemyVector *e, unsigned char n);		// remove enemy from vector
unsigned char numEnemies(EnemyVector *e);				// return number of enemies in vector



/////////////////////////////////////////////////
// SETUP
/////////////////////////////////////////////////

void setup()
{
	  randomSeed(analogRead(A1));	// randomize with pin 1
	  initDisplay();				// initialize TFT display

	  tft.print(F("Bitte das Spiel gerade halten,\nich kalibriere...\n"));
	  delay(1500);
	  gyroscope.calibrate();		// calibrate gyroscope
	  tft.print(F("fertig.\n"));

	  initSDCard();					// initialize SD card

	  gamefield.init(5, 25, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 30, COLOR_BLUE, COLOR_YELLOW);  // set up game field
	  player.init(gamefield.getLeft() + (gamefield.getWidth() / 2), gamefield.getTop() + (gamefield.getHeight() / 2), COLOR_GREEN);  // set up player
	  player.setRadius(7);

	  timer.init(60);

	  statusBar.init(0, 0, SCREEN_WIDTH, 19, COLOR_BLACK, COLOR_WHITE);

	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy
	  addEnemy(&enemies);			// add an enemy

	  gamefield.draw();
	  player.draw();
	  timer.start();
}



/////////////////////////////////////////////////
// LOOP
/////////////////////////////////////////////////

void loop()
{
	gyroscope.read();
	timer.update();

	player.setSpeed(gyroscope.getSpeed());
	player.move();
	moveEnemies(&enemies);

	player.draw();
	drawEnemies(&enemies);
	statusBar.draw();
	//delay(5);
	//Waveshield.drawBitmap(0, 0, canvas.getBuffer(), 480, 320, 1, 0); //

	if (timer.expired()) player.setColor(COLOR_RED);
}



/////////////////////////////////////////////////
// FUNCTION IMPLEMANTATION
/////////////////////////////////////////////////

void initDisplay() {
	SPI.begin();
	Waveshield.begin();
	tft.setRotation(3);
};


void initSDCard() {
	if (!  /*card.init(SPI_HALF_SPEED, 5)*/ SD.begin(5))
	{
		tft.print(F("SD-Karte nicht erkannt. Keine Highscore :-(\n"));
		SDCard=false;
		delay(2000);
	} else {
		tft.print(F("SD-Karte in Ordnung :-)\n"));
		SDCard=true;
	}
}


void randomPositionEnemy(Enemy *e) {
	unsigned char dx, dy;
	dx = random(3,5);
	dy = random(1,3);
	e->setDirection(dx, dy);
	e->setPosition(random(gamefield.getLeft() + e->getRadius(), gamefield.getLeft() + gamefield.getWidth()  - e->getRadius()),
			       random(gamefield.getTop()  + e->getRadius(), gamefield.getTop()  + gamefield.getHeight() - e->getRadius()));
};


void drawEnemies(EnemyVector *e) {
	for (unsigned char i = 0; i < numEnemies(e); ++i) e->at(i).draw();
};


void moveEnemies(EnemyVector *e) {
	for (unsigned char i = 0; i < numEnemies(e); ++i) e->at(i).move();
};


void addEnemy(EnemyVector *e) {
	Enemy newEnemy(&gamefield, &tft);
	randomPositionEnemy(&newEnemy);
	//if (e->capacity() > e->size()+1)
		e->push_back(newEnemy);
};


void removeEnemy(EnemyVector *e, unsigned char n) {
	if (n < e->size()) e->erase(e->begin()+n);
};


unsigned char numEnemies(EnemyVector *e) {
	return e->size();
};
