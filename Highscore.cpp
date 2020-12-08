#include "Highscore.h"


Highscore::Highscore(Adafruit_GFX *s)
: screen(s), currentScore(0)
{};


Highscore::~Highscore() {};


void Highscore::showHighscore() {
	readTable();
	display();
};


void Highscore::processHighscore(unsigned short int curScore) {
	screen->fillScreen(COLOR_BLUE);
	screen->print(F("lade Highscore ...\n"));
	currentScore = curScore;
	readTable();
	delay(100);
	score[20] = currentScore;
	sort();
	screen->print(F("speichere Highscore ...\n"));
	writeTable();
	delay(100);
	display();
	do {} while (0==0); //never ending loop. New game by reset!
};


void Highscore::sort() {
	unsigned short int temp;

	for(unsigned int i = 0; i <= 19; i++) { // walk through the array, but don't test the last element
		unsigned int min_pos = i;  // Position last smallest element

	    // walk through unsorted part of array and look for the smallest element
	    for (unsigned int j = i + 1; j <= 20; j++)
	    	if (score[j] > score[min_pos]) min_pos = j;

	    // swap elements: smallest one to the end of sorted part of array
        temp = score[i];
        score[i] = score[min_pos];
        score[min_pos] = temp;
	};
};


void Highscore::display() {
	readTable();
	screen->fillScreen(COLOR_BLACK);
	screen->setTextColor(COLOR_YELLOW);
	screen->setCursor(50, 30);
	screen->print(F("HIGHSCORE"));
	screen->setCursor(100, 80);
	screen->setTextColor(COLOR_WHITE);
	screen->print(F("deine Punkte:"));
	screen->setCursor(100, 94);
	screen->print(currentScore * 10);
	for (unsigned char i = 0; i <= 20; i++) {
		if (score[i] == currentScore) screen->setTextColor(COLOR_GREEN); // highlight current score
			else screen->setTextColor(COLOR_WHITE);
		screen->setCursor(370 - i * 15, 50 + i * 12);
		screen->print(score[i] * 10);
	};
};


void Highscore::writeEmptyTable() {
	writeTable();
};


void Highscore::readTable() {
	int adr = 0;

	for (unsigned char i = 0; i <= 20; i++) {
		score[i] = EEPROM.read(adr);
		adr ++;
	};
};


void Highscore::writeTable() {
	int adr = 0;

	for (unsigned char i = 0; i <= 20; i++) {
		EEPROM.update(adr, score[i]);
		adr ++;
	};
};


