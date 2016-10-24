//FILE : enemyclass.cpp
//PROG : Adam Barker 
//PURP : Implementations of the enemyclass objects

#include "scrollerDefs.h"
#include "Enemyclass.h"

Enemyclass::Enemyclass() 
{
	ex = 50.0;
	ey = 60.0;
	oldex = ex;
	oldey = ey;
	enemy_width = 97;
	height = 86;
	collisionTimer = 0;
	facingRight = true;
	curframe = 0;
	maxframe = 3;
	framecount = 0;
	framedelay = 12;
	verticalSpeed = 0.0;

	ALLEGRO_BITMAP *temp = al_load_bitmap("alien.bmp");
	enemyHit = al_load_bitmap("alienHit.bmp");

	al_convert_mask_to_alpha(temp, al_map_rgb(255, 0, 255));
	al_convert_mask_to_alpha(enemyHit, al_map_rgb(255, 0, 255));

	for (int ct = 0; ct < 4; ++ct)
	{
		enemy_image[ct] = al_create_bitmap(enemy_width, height);
		al_set_target_bitmap(enemy_image[ct]);
		int sourceX = 0;
		int sourceY = 0;
		int columns = 4;
		al_draw_bitmap_region(temp, (ct % columns) * enemy_width, (ct / columns) * height, enemy_width, height, 0, 0, 0);
	}

	al_destroy_bitmap(temp);
}//END constructor

Enemyclass::~Enemyclass()
{
	for (int ct = 0; ct < 4; ++ct)
	{
		al_destroy_bitmap(enemy_image[ct]);
	}
}

void Enemyclass::updateEnemy(bool sCollide, bool pColl)
{
	oldex = ex;
	oldey = ey;

	if (ex <= 0)
		facingRight = false;

	if (pCollisionTimer < 1)
	{
		pCollide = pColl;

		hitex = oldex;
		hitey = oldey;

		if (pCollide)
		{
			pCollisionTimer = 90;

		}
	}
	else
		pCollisionTimer--;
	
	if (collisionTimer < 1)
	{
		if (sCollide)
		{
			collisionTimer = 90;

			if (facingRight)
				facingRight = false;
			else
				facingRight = true;
		}
	}
	else
		collisionTimer--;

	if (!collided(ex + enemy_width / 2, ey + height + 4))
	{
		ey -= verticalSpeed / 3;
		verticalSpeed -= 1.0;
	}
	else
	{
		verticalSpeed = 0.0;
		//Correction for falling a bit too far...into a floor tile!
		while (collided(ex + enemy_width / 2, ey + height))
			ey -= 2;
	}

	if (verticalSpeed > 0)
	{
		if (collided(ex + 10, ey) || collided(ex + enemy_width - 10, ey))
			verticalSpeed = 0;
	}

	if (!pCollide)
	{
		if (facingRight)
		{
			if (!collided(ex + enemy_width / 10, ey + height))
				ex -= 2;
			else
			{
				ex = oldex;
				facingRight = false;
			}

			if (++framecount > framedelay)
			{
				framecount = 0;
				if (++curframe > maxframe)
					curframe = 0;
			}
		}
		else
		{
			if (!collided(ex + enemy_width, ey + height))
				ex += 2;

			else
			{
				ex = oldex;
				facingRight = true;
			}
			if (++framecount > framedelay)
			{
				framecount = 0;
				if (++curframe > maxframe)
					curframe = 0;
			}

		}
	}
}

void Enemyclass::drawEnemy(int mapxoff, int mapyoff)
{
	if (!pCollide)
	{
		if (facingRight)
			al_draw_bitmap(enemy_image[curframe], (ex - mapxoff), (ey - mapyoff), 0);
		else
			al_draw_bitmap(enemy_image[curframe], (ex - mapxoff), (ey - mapyoff),
				ALLEGRO_FLIP_HORIZONTAL);
	}
	else
	{
		if (facingRight)
			al_draw_bitmap(enemyHit, (hitex - mapxoff), (hitey - mapyoff), 0);
		else
			al_draw_bitmap(enemyHit, (hitex - mapxoff), (hitey - mapyoff),
				ALLEGRO_FLIP_HORIZONTAL);
	}
}//END drawEnemy

void Enemyclass::setex(float x)
{
	ex = x;
}

bool Enemyclass::spriteCollide(int pw, int px, int py)
{
	int collPWidth = pw * 0.80;
	int collEWidth = enemy_width * 0.80;
	int collHeight = height * 0.80;
	int collPXOffset = (pw - collPWidth) / 2.0;
	int collEXOffset = (enemy_width - collEWidth) / 2.0;
	int collYOffset = (height - collHeight) / 2.0;

	int left1, left2;
	int right1, right2;
	int top1, top2;
	int bottom1, bottom2;

	left1 = px + collPXOffset;
	left2 = ex + collEXOffset;
	right1 = left1 + collPWidth;
	right2 = left2 + collEWidth;
	top1= py + collYOffset;
	top2 = ey + collYOffset;
	bottom1 = top1 + collHeight;
	bottom2 = top2 + collHeight;

	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;
	if (right1 < left2) return false;
	if (left1 > right2) return false;
	return true;
}



