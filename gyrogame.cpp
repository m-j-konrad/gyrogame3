#include "gyrogame.h"



/////////////////////////////////////////////////
// CLASS GAMEFIELD
/////////////////////////////////////////////////

Gamefield::Gamefield(Adafruit_GFX *s)
: screen(s)
{};


void Gamefield::init(short int left, short int top, short int width, short int height, unsigned short int color, unsigned short int borderColor) {
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->color = color;
	this->borderColor = borderColor;
};


short int Gamefield::getLeft() {
	return left;
};


short int Gamefield::getTop() {
	return top;
};


short int Gamefield::getWidth() {
	return width;
};


short int Gamefield::getHeight() {
	return height;
};


unsigned short int Gamefield::getColor() {
	return color;
};


void Gamefield::draw() {
	screen->fillRect(left, top, width, height, color);
	screen->drawRect(left - 1, top - 1, width + 2, height + 2, borderColor);

};



/////////////////////////////////////////////////
// CLASS PLAYER
/////////////////////////////////////////////////

Player::Player(Gamefield *g, Adafruit_GFX *s)
: gamefield(g), screen(s)
{};


void Player::init(short int x, short int y, unsigned short col) {
	pos.x = x;
	pos.y = y;
	oldPos.x = x + 1;
	oldPos.y = y + 1;
	color = col;
	width = 10;
};


void Player::setWidth(short int w) {
	width = w;
}


void Player::setColor(unsigned short int c) {
	color = c;
}


void Player::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
};


void Player::setSpeed(SpeedT s) {
	speed = s;
}


void Player::setName() {  // TO DO: Change name with touch menu

};


void Player::move() {
	oldPos.x = pos.x;
	oldPos.y = pos.y;
	// move on x-axis
	pos.x += speed.x;
	// if out of borders, set position to border
	if (pos.x < gamefield->getLeft()) pos.x = gamefield->getLeft();
	if ((pos.x + width) >= (gamefield->getWidth() + gamefield->getLeft()))
		pos.x = gamefield->getWidth() + gamefield->getLeft() - width - 1;
	//move on y-axis
	pos.y += speed.y;
	// if out of borders, set position to border
	if (pos.y < gamefield->getTop()) pos.y = gamefield->getTop();
	if ((pos.y + width) >= (gamefield->getHeight() + gamefield->getTop()))
		pos.y = gamefield->getHeight() + gamefield->getTop() - width - 1;
};


void Player::draw() {
	if ((oldPos.x != pos.x) || (oldPos.y != pos.y)) {
		screen->fillCircle(oldPos.x + width / 2, oldPos.y + width / 2, width / 2, gamefield->getColor());	// overdraw old position with background color
		screen->fillCircle(pos.x + width / 2, pos.y + width / 2, width / 2, color);					// draw at new position
		//screen->fillRect(oldPos.x, oldPos.y, width, width, gamefield->getColor());
		//screen->fillRect(pos.x,    pos.y,    width, width,    color);
	}
};


short int Player::getX() {
	return pos.x;
};


short int Player::getY() {
	return pos.y;
};


short int Player::getWidth() {
	return width;
};


void Player::addScore(short int value) {
	scoreOld = score;
	// the score will not be negative.
	if (score + value >= 0) score += value;
	else score = 0;
};


short int Player::getScore() {
	return score;
};


bool Player::scoreChanged() {
	if (scoreOld != score) {
		scoreOld = score;
		return true;
	} else {
		scoreOld = score;
		return false;
	};
};



/////////////////////////////////////////////////
// CLASS ENEMY
/////////////////////////////////////////////////

Enemy::Enemy(Gamefield *g, Adafruit_GFX *s)
: gamefield(g), screen(s)
{
	width=5;
	randomPosition();
};


void Enemy::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
	// check if the position is inside the game field
	if (x < gamefield->getLeft()) x = gamefield->getLeft() + 5;
	if (y < gamefield->getTop())  x = gamefield->getTop()  + 5;
	if (x + width > gamefield->getLeft() + gamefield->getWidth()) x = gamefield->getLeft() + gamefield->getWidth() - width - 5;
	if (x + width > gamefield->getTop() + gamefield->getHeight()) x = gamefield->getTop() + gamefield->getHeight() - width - 5;
};


void Enemy::randomPosition() {
	unsigned char dx, dy;
	dx = random(3,5);
	dy = random(1,3);
	setDirection(dx, dy);
	setPosition(random(gamefield->getLeft() + 5, gamefield->getLeft() + gamefield->getWidth()  - width - 5),
			    random(gamefield->getTop() + 5,  gamefield->getTop()  + gamefield->getHeight() - width - 5));
};


void Enemy::setDirection(unsigned char x, unsigned char y) {
	dir.x = x;
	dir.y = y;
};


void Enemy::setSpeed(unsigned char s) {
	speed = s;
};


void Enemy::setWidth(short int w) {
	width = w;
};


void Enemy::move() {
	// save old position
	oldPos.x = pos.x;
	oldPos.y = pos.y;
	// if movement would exceed game field borders then change direction
	if ((dir.x == DIR_LEFT)  && ((pos.x - speed) < gamefield->getLeft())) dir.x = DIR_RIGHT;
	if ((dir.y == DIR_UP)    && ((pos.y - speed) < gamefield->getTop())) dir.y = DIR_DOWN;
	if ((dir.x == DIR_RIGHT) && ((pos.x + speed) > (gamefield->getLeft() + gamefield->getWidth()  - width))) dir.x = DIR_LEFT;
	if ((dir.y == DIR_DOWN)  && ((pos.y + speed) > (gamefield->getTop()  + gamefield->getHeight() - width))) dir.y = DIR_UP;
	// move
	if (dir.x == DIR_LEFT)  pos.x -= speed;
	if (dir.y == DIR_UP)    pos.y -= speed;
	if (dir.x == DIR_RIGHT) pos.x += speed;
	if (dir.y == DIR_DOWN)  pos.y += speed;
};


void Enemy::draw() {
	if ((oldPos.x != pos.x) || (oldPos.y != pos.y)) {
		// too many circles will be flickering because there's no frame buffer, so enemies are rectangles now!
		//screen->fillCircle(oldPos.x, oldPos.y, width * 2, gamefield->getColor());
		//screen->fillCircle(pos.x, pos.y, width * 2, color);
		screen->fillRect(oldPos.x, oldPos.y, width, width, gamefield->getColor());
		if (friendly) screen->fillRect(pos.x, pos.y, width, width, COLOR_GREEN);
		else screen->fillRect(pos.x, pos.y, width, width, COLOR_RED);
	}
};


void Enemy::unDraw() {
	screen->fillRect(pos.x, pos.y, width, width, gamefield->getColor());
}


short int Enemy::getX() {
	return pos.x;
};


short int Enemy::getY() {
	return pos.y;
};


short int Enemy::getWidth() {
	return width;
};


bool Enemy::isFriendly() {
	return friendly;
};


void Enemy::makeFriendly() {
	friendly = true;
	width *= 2;	//bonuses should be a little bigger, easier to catch.
};



/////////////////////////////////////////////////
// CLASS GYROSCOPE
/////////////////////////////////////////////////

Gyroscope::Gyroscope() {};
Gyroscope::~Gyroscope() {};


void Gyroscope::read() {
	x = analogRead(GYROSCOPE_PIN_X) - calibX;
	y = analogRead(GYROSCOPE_PIN_Y) - calibY;
	z = analogRead(GYROSCOPE_PIN_Z) - calibZ;

	speed.x = -(x / 3);
	speed.y = -(y / 3);
};


void Gyroscope::calibrate() {
	read();
	calibX = x;
	calibY = y;
	calibZ = z;
};


SpeedT Gyroscope::getSpeed() {
	return speed;
}


unsigned char Gyroscope::getDirectionX() {
	read();
	switch (speed.x) {
		case -4: return DIR_LEFT;
		case 4: return DIR_RIGHT;
		case 0: return DIR_NODIRECTION;
		default: return DIR_NODIRECTION;
	}
};


unsigned char Gyroscope::getDirectionY() {
	read();
	switch (speed.y) {
		case -4: return DIR_UP;
		case 4: return DIR_DOWN;
		case 0: return DIR_NODIRECTION;
		default: return DIR_NODIRECTION;
	}
};



/////////////////////////////////////////////////
// CLASS COUNTDOWN
/////////////////////////////////////////////////

Countdown::Countdown()
: pauseTimeStarted(0), timeStarted(0)
{};
Countdown::~Countdown() {};


void Countdown::init(unsigned int s) {
	seconds = s;
	pauseTime = 0;
	paused = false;
};


void Countdown::start() {
	timeLeft = seconds;
	timeStarted = millis();
};


// there's no pause implemented. Will not really work
void Countdown::togglePause() {
	if (!paused) {
		paused = true;
		pauseTimeStarted = millis();
	} else {
		paused = false;
	}
};


// pause won't really work for now
void Countdown::update() {
	timeLeftOld = timeLeft;
	if (paused) pauseTime = (millis() - pauseTimeStarted) / 1000;
	timeLeft = seconds + pauseTime - ((millis() - timeStarted) / 1000);
	if (timeLeft < 0) timeLeft = 0;
};


bool Countdown::expired() {
	if (timeLeft > 0) return false;
	else return true;
};


unsigned int Countdown::getTimeLeft() {
	return timeLeft;
};


unsigned int Countdown::getTimeLeftOld() {
	return timeLeftOld;
};



/////////////////////////////////////////////////
// CLASS STATUSBAR
/////////////////////////////////////////////////

StatusBar::StatusBar(Player *p, Countdown *c, Adafruit_GFX *s)
: player(p), countdown(c), screen(s)
{};

void StatusBar::init(short int left, short int top, short int width, short int height, unsigned short int bkColor, unsigned short int borderColor) {
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->bkColor = bkColor;
	this->borderColor = borderColor;
};


void StatusBar::draw() {
	char buff[6];

	if (countdown->getTimeLeft() != countdown->getTimeLeftOld()) {
		itoa(countdown->getTimeLeft(), buff, 10);
		screen->setCursor(left + 50, top + height - 7);
		if (countdown->getTimeLeft() > 10) screen->setTextColor(COLOR_GREEN);
		else screen->setTextColor(COLOR_RED);
		screen->fillRect(left + 50, top + 2, left + 80, top + height - 3, bkColor);
		screen->print(buff);
	};

	if (player->scoreChanged()) {
		itoa(player->getScore() * 10, buff, 10);
		screen->setCursor(left + width - 60, top + height - 7);
		screen->setTextColor(COLOR_WHITE);
		screen->fillRect(left + width - 60, top + 2, 30, height - 3, bkColor);
		screen->print(buff);
		//player->addScore(0);	//Update scoreOld, to not redraw the score continuously
	};
};


void StatusBar::drawAll() {
	char buff[6];

	screen->fillRect(left + 1, top + 1, width - 1, height - 1, bkColor);
	screen->drawRect(left, top, width, height, borderColor);

	screen->setCursor(left + 5, top + height - 7);
	screen->setTextColor(borderColor);
	screen->print(F("Zeit:"));
	itoa(countdown->getTimeLeft(), buff, 10);
	screen->setCursor(left + 50, top + height - 7);
	screen->setTextColor(COLOR_GREEN);
	screen->print(buff);

	screen->setCursor(left + width - 130, top + height - 7);
	screen->setTextColor(borderColor);
	screen->print(F("Punkte:"));
	itoa(player->getScore(), buff, 10);
	screen->setCursor(left + width - 60, top + height - 7);
	screen->setTextColor(COLOR_WHITE);
	screen->print(buff);
};



/////////////////////////////////////////////////
// CLASS ENEMYFIELD
/////////////////////////////////////////////////

EnemyField::EnemyField(Player *p, Gamefield *g, Adafruit_GFX *s, unsigned short int nE)
:player(p), gamefield(g), screen(s), numE(nE)
{};


EnemyField::~EnemyField() {};


void EnemyField::draw() {
	for (unsigned char i = 0; i < numEnemies(); i++) e.at(i).draw();
};


void EnemyField::move() {
	for (unsigned char i = 0; i < numEnemies(); i++) e.at(i).move();
};


void EnemyField::addEnemy(bool f) {
	Enemy newEnemy(gamefield, screen);
	bool positionOk = true;

	do {
		positionOk = true;
		newEnemy.randomPosition();
		for (unsigned short int i = 0; i < numEnemies(); i++)
			if ((newEnemy.getX() + newEnemy.getWidth() * 3) > (player->getX()) &&
				(newEnemy.getY() + newEnemy.getWidth() * 3) > (player->getY()) &&
				(newEnemy.getX() - newEnemy.getWidth() * 2) < (player->getX() + player->getWidth()) &&
				(newEnemy.getY() - newEnemy.getWidth() * 2) < (player->getY() + player->getWidth())) positionOk = false;
	} while (!positionOk);

	if (f) {
		newEnemy.makeFriendly();
		newEnemy.setSpeed(2);
	}

	//newEnemy.setWidth(3);
	//if (e->capacity() > e->size()+1)
	e.push_back(newEnemy);
};


void EnemyField::removeEnemy(unsigned char n) {
	if (n < e.size()) {
		e.at(n).unDraw();
		e.erase(e.begin()+n);
	}
};


unsigned char EnemyField::numEnemies() {
	return e.size();
};


void EnemyField::collisionTest() {
	for (unsigned char i = 0; i < numEnemies(); ++i)
		if ((player->getX() < (e.at(i).getX() + e.at(i).getWidth())) &&
			(player->getY() < (e.at(i).getY() + e.at(i).getWidth())) &&
			((player->getX() + player->getWidth()) > e.at(i).getX()) &&
			((player->getY() + player->getWidth()) > e.at(i).getY()))
		{
			if (e.at(i).isFriendly()) {
				player->addScore(1);
				removeEnemy(i);
				addEnemy(true);
				addEnemy(false);
			}
			else {
				player->addScore(-3);
				removeEnemy(i);
			};
		};
};