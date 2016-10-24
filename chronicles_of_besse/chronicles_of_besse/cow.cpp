//FILE : cow.cpp
//PROG : Adam Barker
//PURP : Implementations of the cow class objects

#include "cow.h"

Cow::Cow(int mapblockwidth, int mapblockheight, int mapwidth)
{
	cWidth = 64;
	cHeight = 64;
	cx = mapblockwidth * (mapwidth - 1) - cWidth;
	cy = mapblockheight * 8 - cHeight;
	curframe = 0;
	maxframe = 3;
	framecount = 0;
	framedelay = 12;
	height = 86;

	ALLEGRO_BITMAP *temp = al_load_bitmap("cow.bmp");

	al_convert_mask_to_alpha(temp, al_map_rgb(255, 0, 255));
	for (int ct = 0; ct < 4; ++ct)
	{
		cow_image[ct] = al_create_bitmap(cWidth, cHeight);
		al_set_target_bitmap(cow_image[ct]);
		int sourceX = 0;
		int sourceY = 0;
		int columns = 4;
		al_draw_bitmap_region(temp, (ct % columns) * cWidth, (ct / columns) * cHeight, cWidth, cHeight, 0, 0, 0);
	}

	al_destroy_bitmap(temp);
}

Cow::~Cow()
{
	for (int ct = 0; ct < 4; ++ct)
	{
		al_destroy_bitmap(cow_image[ct]);
	}
}

void Cow::updateCow()
{
	if (++framecount > framedelay)
	{
		framecount = 0;
		if (++curframe > maxframe)
			curframe = 0;
	}
}

bool Cow::cowCollide(int pw, int px, int py)
{
	int collCWidth = cWidth * 0.80;
	int collPWidth = pw * 0.80;
	int collCHeight = cHeight * 0.80;
	int collPHeight = height * 0.80;
	int collCXOffset = (cWidth - collPWidth) / 2.0;
	int collPXOffset = (pw - collPWidth) / 2.0;
	int collCYOffset = (cHeight - collCHeight) / 2.0;
	int collPYOffset = (height - collPHeight) / 2.0;

	int left1, left2;
	int right1, right2;
	int top1, top2;
	int bottom1, bottom2;

	left1 = cx + collCXOffset;
	left2 = px + collPXOffset;
	right1 = left1 + collCWidth;
	right2 = left2 + collPWidth;
	top1 = cy + collCYOffset;
	top2 = py + collPYOffset;
	bottom1 = top1 + collCHeight;
	bottom2 = top2 + collPHeight;

	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;
	if (right1 < left2) return false;
	if (left1 > right2) return false;
	return true;
}

void Cow::drawCow(int mapxoff, int mapyoff)
{
	al_draw_bitmap(cow_image[curframe], (cx - mapxoff), (cy - mapyoff), 0);
}
