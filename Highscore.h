#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_


#include "Arduino.h"
#include "ArduinoSTL.h"
#include "EEPROM.h"
#include <Adafruit_GFX.h>
#include "gyrogame.h"
#include <string.h>


class Highscore {
	public:
		Highscore(Adafruit_GFX *s);
		virtual ~Highscore();
		void processHighscore(unsigned short int curScore);
		void showHighscore();
		void writeEmptyTable();
	private:
		void readTable();
		void writeTable();
		void display();
		void sort();

		Adafruit_GFX *screen;
		Gyroscope gyro;
		unsigned short int score[21] {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
		unsigned short int currentScore;
};

#endif /* HIGHSCORE_H_ */
