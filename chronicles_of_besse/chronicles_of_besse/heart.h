//FILE : heart.h
//PROG : Adam Barker 
//PURP : Class for the heart/health system

#include "allegro5\allegro.h"

class Heart
{
private:
	float heartXOne, heartXTwo, heartXThree, heartY;
	float oldXOne, oldXTwo, oldXThree;
	int heartWidth, heartHeight;
	int collisionCounter;
	int collisionTimer;
	bool quit;
	ALLEGRO_BITMAP *heart;

public:
	Heart();
	~Heart();
	bool getQuit() { return quit; }
	void updateHeart(int, bool);
	void drawHeart();
};
