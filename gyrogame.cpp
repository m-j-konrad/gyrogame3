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
	screen->fillRect(left, top, left + width, top + height, color);
	screen->drawRect(left - 1, top - 1, left + width + 1, top + height + 1, borderColor);

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
};

void Player::setRadius(short int r) {
	radius = r;
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
	if (pos.x < radius + gamefield->getLeft()) pos.x = radius + gamefield->getLeft();
	if (pos.x > gamefield->getWidth() + gamefield->getLeft() - radius)
		pos.x = gamefield->getWidth() + gamefield->getLeft() - radius;
	//move on y-axis
	pos.y += speed.y;
	// if out of borders, set position to border
	if (pos.y < radius + gamefield->getTop()) pos.y = radius + gamefield->getTop();
	if (pos.y > gamefield->getHeight() + gamefield->getTop() - radius)
		pos.y = gamefield->getHeight() + gamefield->getTop() - radius;
};


void Player::draw() {
	if ((oldPos.x != pos.x) || (oldPos.y != pos.y)) {
		screen->fillCircle(oldPos.x, oldPos.y, radius, gamefield->getColor());	// overdraw old position with background color
		screen->fillCircle(pos.x, pos.y, radius, color);					// draw at new position
	}
};


short int Player::getX() {
	return pos.x;
};


short int Player::getY() {
	return pos.y;
};


void Player::addScore(short int value) {
	if (score + value > 0) score += value;
	else score = 0;
};


unsigned int Player::getScore() {
	return score;
};



/////////////////////////////////////////////////
// CLASS ENEMY
/////////////////////////////////////////////////

Enemy::Enemy(Gamefield *g, Adafruit_GFX *s)
: gamefield(g), screen(s)
{};


void Enemy::init(short int x, short int y, unsigned short int col) {
	pos.x = x;
	pos.y = y;
	oldPos.x = x + 1;
	oldPos.y = y + 1;
	color = col;
};


void Enemy::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
	if (x < gamefield->getLeft() + radius) x = gamefield->getLeft() + radius;
	if (y < gamefield->getTop() + radius) x = gamefield->getTop() + radius;
	if (x > gamefield->getLeft() + gamefield->getWidth() - radius) x = gamefield->getLeft() + gamefield->getWidth() - radius;
	if (x > gamefield->getTop() + gamefield->getHeight() - radius) x = gamefield->getTop() + gamefield->getHeight() - radius;
};


void Enemy::setDirection(unsigned char x, unsigned char y) {
	dir.x = x;
	dir.y = y;
};


void Enemy::setSpeed(unsigned char s) {
	speed = s;
};


void Enemy::setRadius(short int r) {
	radius = r;
};


void Enemy::move() {
	// save old position
	oldPos.x = pos.x;
	oldPos.y = pos.y;
	// if movement would exceed game field borders then change direction
	if ((dir.x == DIR_LEFT)  && ((pos.x - speed) < gamefield->getLeft()  + radius))                           dir.x = DIR_RIGHT;
	if ((dir.y == DIR_UP)    && ((pos.y - speed) < gamefield->getTop()   + radius))                           dir.y = DIR_DOWN;
	if ((dir.x == DIR_RIGHT) && ((pos.x + speed) > (gamefield->getLeft() + gamefield->getWidth()  - radius))) dir.x = DIR_LEFT;
	if ((dir.y == DIR_DOWN)  && ((pos.y + speed) > (gamefield->getTop()  + gamefield->getHeight() - radius))) dir.y = DIR_UP;
	// move
	if (dir.x == DIR_LEFT)  pos.x -= speed;
	if (dir.y == DIR_UP)    pos.y -= speed;
	if (dir.x == DIR_RIGHT) pos.x += speed;
	if (dir.y == DIR_DOWN)  pos.y += speed;
};


void Enemy::draw() {
	if ((oldPos.x != pos.x) || (oldPos.y != pos.y)) {
		screen->drawCircle(oldPos.x, oldPos.y, radius, gamefield->getColor());
		screen->drawCircle(pos.x, pos.y, radius, color);
	}
};


short int Enemy::getX() {
	return pos.x;
};


short int Enemy::getY() {
	return pos.y;
};


short int Enemy::getRadius() {
	return radius;
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


void Countdown::togglePause() {
	if (!paused) {
		paused = true;
		pauseTimeStarted = millis();
	} else {
		paused = false;
		pauseTime = 0;
	}
};


void Countdown::update() {
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

	strcpy(caption, "was weiß ich\0");
	char buff[10];
	itoa(countdown->getTimeLeft(), buff, 10);
	strcpy(caption, buff);

	screen->fillRect(left + 1, top + 1, left + width - 1, top + height - 1, bkColor);
	screen->drawRect(left, top, left + width, top + height, borderColor);
	centerText(screen, caption, left, top, width, height, borderColor, 2);
};



/////////////////////////////////////////////////
// HELPER FUNCTIONS
/////////////////////////////////////////////////

unsigned short rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
};


void centerText(Adafruit_GFX *scr, char txt[], short int l, short int t, short int w, short int h, unsigned short color, unsigned short size) {
	int16_t  x1 = l, y1 = t;
	uint16_t width, height;

	scr->setTextColor(color);
	scr->setTextSize(size);
	scr->getTextBounds(txt, l, t, &x1, &y1, &width, &height);
	scr->setCursor(l + (w / 2) - (width / 2), t + (h / 2) - (height / 2));
	scr->print(txt);
};

