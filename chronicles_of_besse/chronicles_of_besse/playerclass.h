//FILE : playerclass.h
//PROG : Adam Barker 
//PURP : Class for the single player sprite

#include "spriteclass.h"

class Playerclass : Sprite
{
private:
	float x, y, oldpx, oldpy;
	int player_width;
	ALLEGRO_BITMAP *player_image[4];
	ALLEGRO_BITMAP *playerHit;
	ALLEGRO_SAMPLE *jumpSound;
	ALLEGRO_SAMPLE *hitSound;
	ALLEGRO_SAMPLE_ID soundID;

public:
	Playerclass(int, int, int);
	~Playerclass();
	float getX() { return x; }
	float getY() { return y; }
	int getPlayerWidth() { return player_width; }
	int getHeight() { return height; }
	bool getFacing() { return facingRight; }
	void updatePlayer(bool);
	void drawPlayer(int, int);
};
