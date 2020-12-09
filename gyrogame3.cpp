#include "ArduinoSTL.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Waveshare4InchTftShield.h>
#include "font_coming_soon_12pt.h"
#include "Highscore.h"
#include "gyrogame.h"

Waveshare4InchTftShield Waveshield;
Adafruit_GFX &tft = Waveshield;


unsigned int screenWidth	= 480;		// Change for other displays
unsigned int screenHeight	= 320;

Gyroscope gyroscope;
Gamefield gamefield(&tft);
Player player(&gamefield, &tft);
EnemyField enemies(&player, &gamefield, &tft, 10);
Countdown timer;		// timer to count down
StatusBar statusBar(&player, &timer, &tft);	// status bar
Highscore highscore(&tft);
bool starWarsMode = false; // to enable "star wars mode"; lets the enemies move!

void initDisplay();										// initialize TFT display (hardware, rotation)


/////////////////////////////////////////////////
// SETUP
/////////////////////////////////////////////////

void setup()
{
	randomSeed(analogRead(A1));	// randomize with pin 1
	initDisplay();				// initialize TFT display


	tft.print(F("  GYROGAME III\n\nBitte das Spiel gerade halten,\nich kalibriere...\n"));
	delay(1500);
	gyroscope.calibrate();		// calibrate gyroscope
	tft.print(F("fertig.\n\n\n\n\n"));
	delay(100);
	tft.print(F("Bitte neige das Spiel um den Modus zu bestimmen:\n"));
	tft.print(F("LINKS: normales Spiel\n"));
	tft.print(F("RECHTS: Krieg-der-Sterne-Modus"));

	do {
	} while (gyroscope.getDirectionX() == DIR_NODIRECTION);
	if (gyroscope.getDirectionX() == DIR_LEFT) starWarsMode = false;
	if (gyroscope.getDirectionX() == DIR_RIGHT) starWarsMode = true;

	statusBar.init(0, 0, screenWidth, 24, COLOR_BLACK, COLOR_WHITE);
	gamefield.init(5, 26, screenWidth - 10, screenHeight - 28, COLOR_BLUE, COLOR_YELLOW);  // set up game field
	player.init(gamefield.getLeft() + (gamefield.getWidth() / 2), gamefield.getTop() + (gamefield.getHeight() / 2), COLOR_GREEN);  // set up player
	player.setWidth(20);

	timer.init(60);

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
	if (starWarsMode == true) enemies.move();
	enemies.draw();
	player.draw();
	statusBar.draw();

	timer.update();
	enemies.collisionTest();

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
