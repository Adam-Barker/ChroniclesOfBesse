//FILE : spriteclass.h
//PROG : Adam Barker (prototype: Dave Jones)
//PURP : Class for all sprites

#ifndef SPRITECLASS_H
#define SPRITECLASS_H
#include "allegro5\allegro.h"
#include "mappy_A5.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class Sprite
{
protected:
	int mapx, mapy;
	int height;
	bool facingRight;
	bool eCollided;
	int collisionTimer;
	float verticalSpeed;
	int curframe, maxframe;
	int framecount, framedelay;


public:
	unsigned char collided(float, float);
};
#endif
