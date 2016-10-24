//FILE : cow.h
//PROG : Adam Barker 
//PURP : Class for cow sprite

#include "spriteclass.h"


class Cow : Sprite
{
private: 
	int cx, cy, cWidth, cHeight;
	ALLEGRO_BITMAP *cow_image[4];

public:
	Cow(int, int, int);
	~Cow();
	bool cowCollide(int, int, int);
	void updateCow();
	void drawCow(int, int);
};