//FILE : heart.cpp
//PROG : Adam Barker
//PURP : Implementations of the health system

#include "heart.h"

Heart::Heart() 
{
	heartXOne = 15;
	heartXTwo = 45;
	heartXThree = 75;
	oldXOne = heartXOne;
	oldXTwo = heartXTwo;
	oldXThree = heartXThree;
	heartY = 30;
	heartWidth = 16;
	heartHeight = 16;
	collisionCounter = 0;
	collisionTimer = 0;
	quit = false;

	heart = al_load_bitmap("heart.bmp");
	al_convert_mask_to_alpha(heart, al_map_rgb(255, 0, 255));
}

Heart::~Heart() 
{
	al_destroy_bitmap(heart);
}

void Heart::updateHeart(int x, bool collided)
{
	heartXOne = x + 50;
	heartXTwo = x + 80;
	heartXThree = x + 110;

	if (collisionTimer < 1)
	{
		if (collided)
		{
			collisionCounter += 1;
			collisionTimer = 240;
		}
	}
	else
		collisionTimer--;

	if (collisionCounter == 3)
		quit = true;
}

void Heart::drawHeart()
{	
	al_draw_bitmap(heart, heartXOne, heartY, 0);
	if(collisionCounter < 2)
		al_draw_bitmap(heart, heartXTwo, heartY, 0);
	if(collisionCounter < 1)
		al_draw_bitmap(heart, heartXThree, heartY, 0);
}


