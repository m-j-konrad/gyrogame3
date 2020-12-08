#include "ArduinoSTL.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Waveshare4InchTftShield.h>
#include "font_coming_soon_12pt.h"
#include "Highscore.h"
#include "gyrogame.h"
#define error(s) sd.errorHalt(F(s))  // store error strings in flash to save RAM
Waveshare4InchTftShield Waveshield;
Adafruit_GFX &tft = Waveshield;

constexpr bool DEBUGGER_ON = false;

unsigned int screenWidth	= 480;		// Change for other displays
unsigned int screenHeight	= 320;

bool SDCard = false;	// is SD card available?
Gyroscope gyroscope;
Gamefield gamefield(&tft);
Player player(&gamefield, &tft);
EnemyField enemies(&player, &gamefield, &tft, 10);
Countdown timer;		// timer to count down
StatusBar statusBar(&player, &timer, &tft);	// status bar
Highscore highscore(&tft);

void initDisplay();										// initialize TFT display (hardware, rotation)


/////////////////////////////////////////////////
// SETUP
/////////////////////////////////////////////////

void setup()
{
	  randomSeed(analogRead(A1));	// randomize with pin 1
	  initDisplay();				// initialize TFT display


	  tft.print(F("  GYROGAME ]I[\n\nBitte das Spiel gerade halten,\nich kalibriere...\n"));
	  delay(1500);
	  gyroscope.calibrate();		// calibrate gyroscope
	  tft.print(F("fertig."));
	  delay(100);

	  statusBar.init(0, 0, screenWidth, 24, COLOR_BLACK, COLOR_WHITE);
	  gamefield.init(5, 26, screenWidth - 10, screenHeight - 28, COLOR_BLUE, COLOR_YELLOW);  // set up game field
	  player.init(gamefield.getLeft() + (gamefield.getWidth() / 2), gamefield.getTop() + (gamefield.getHeight() / 2), COLOR_GREEN);  // set up player
	  player.setWidth(20);

	  timer.init(60);

	  // the very first time the game is running, the highscore table must be created.
	  // TO DO: check for highscore first.
	  //highscore.writeEmptyTable();

	  enemies.addEnemy(true);
	  enemies.addEnemy(true);

	  tft.fillScreen(COLOR_BLACK);
	  gamefield.draw();
	  player.draw();
	  timer.start();
	  statusBar.drawAll();
}



/////////////////////////////////////////////////
// LOOP
/////////////////////////////////////////////////

void loop()
{
	gyroscope.read();
	player.setSpeed(gyroscope.getSpeed());

	player.move();
	//enemies.move();
	enemies.draw();
	player.draw();
	statusBar.draw();

	timer.update();
	enemies.collisionTest();

	if (DEBUGGER_ON) {
		tft.fillRect(300,3,100,12,COLOR_BLACK);
		tft.setCursor(300, 12);
		tft.print(gyroscope.getSpeed().x);
		tft.setCursor(340, 12);
		tft.print(gyroscope.getSpeed().y);
	}

	delay(20); //The more to draw, the less to wait... should work with millis()

	if (timer.expired()) highscore.processHighscore(player.getScore());
}



/////////////////////////////////////////////////
// FUNCTION IMPLEMANTATION
/////////////////////////////////////////////////

void initDisplay() {
	SPI.begin();
	Waveshield.begin();
	tft.setRotation(3);
	tft.setFont(&Coming_Soon_Regular_12);
	screenHeight = tft.height();
	screenWidth = tft.width();
	tft.setCursor(0, 20);
};
