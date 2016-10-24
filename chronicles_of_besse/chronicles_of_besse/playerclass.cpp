//FILE : playersclass.cpp
//PROG : Adam Barker (Prototype: Dave Jones)
//PURP : Implementations of the playerclass objects

//Include basic definitions/prototypes for mappy
//since we need access to functions in method collided
#include "mappy_A5.h"
#include "scrollerDefs.h"
#include "playerclass.h"

//Class needs access to the keys array declared in main.cpp
extern bool keys[];

Playerclass::Playerclass(int maph, int mapbheight, int px)
{
	x = px;
	y = maph * mapbheight - 100;
	oldpx = x;
	oldpy = y;
	collisionTimer = 0;
	facingRight = true;
	player_width = 50;
	height = 86;
	curframe = 0;
	maxframe = 3;
	framecount = 0;
	framedelay = 12;
	//He starts out falling, but initial falling speed is
	//low at the start
	verticalSpeed = 0.0;

	//Load the sprite sheet
	ALLEGRO_BITMAP *temp = al_load_bitmap("farmer.bmp");
	playerHit = al_load_bitmap("playerhit.bmp");
	jumpSound = al_load_sample("jump.wav");
	hitSound = al_load_sample("hit.wav");

	//Convert the background color to be transparent
	al_convert_mask_to_alpha(temp, al_map_rgb(255, 0, 255));
	al_convert_mask_to_alpha(playerHit, al_map_rgb(255, 0, 255));
	//Load the images from the sprite sheet...only first 8 images
	for (int ct = 0; ct < 4; ++ct)
	{
		player_image[ct] = al_create_bitmap(player_width, height);
		al_set_target_bitmap(player_image[ct]);
		int sourceX = 0;
		int sourceY = 0;
		int columns = 4;
		al_draw_bitmap_region(temp, (ct % columns) * player_width, (ct / columns) * height, player_width, height, 0, 0, 0);
	}
	al_destroy_bitmap(temp);
}//END constructor

Playerclass::~Playerclass()
{
	for (int ct = 0; ct < 4; ++ct)
	{
		al_destroy_bitmap(player_image[ct]);
	}
	al_destroy_bitmap(playerHit);
	al_destroy_sample(jumpSound);
	al_destroy_sample(hitSound);
}//end destructor

//This handles all updating, including jumping and "falling", since
//left-right translation needs to continue through vertical movement.
void Playerclass::updatePlayer(bool eCollided)
{
	oldpy = y;
	oldpx = x;

	if (collisionTimer < 140)
	{
		player_width = 50;
		height = 86;

		if (collisionTimer < 1)
		{
			if (eCollided)
			{
				al_play_sample(hitSound, 0.8, 0, 1, ALLEGRO_PLAYMODE_ONCE, &soundID);
				collisionTimer = 180;
			}
		}



		//If he's moving, then curframe ranges from 1 - 7
		//If he's still, then curframe needs to be 0
		if (keys[RIGHT])
		{
			facingRight = true;
			x += 2;
			if (++framecount > framedelay)
			{
				framecount = 0;
				if (++curframe > maxframe)
					curframe = 0;
			}
		}
		else if (keys[LEFT])
		{
			facingRight = false;
			x -= 2;
			if (++framecount > framedelay)
			{
				framecount = 0;
				if (++curframe > maxframe)
					curframe = 0;
			}
		}
		else curframe = 0;

		//Sense spacebar for jumping always, so he can jump while running
		if (keys[SPACEBAR])
		{
			//Only allow jumping if in contact with ground
			if (collided(x + player_width / 2, y + height + 5))
			{
				al_play_sample(jumpSound, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, &soundID);
				//Set a modest vertical speed
				verticalSpeed = 30;
				//Start the jump here, so he won't be in contact with ground
				//using the next decision.
				y -= 10;
			}
			keys[SPACEBAR] = false;
		}
	}
	else
	{
		player_width = al_get_bitmap_width(playerHit);
		height = al_get_bitmap_height(playerHit);
	}
	//If he has not collided with a tile below, then he is either
	//rising or falling. This also handles him falling off a cliff
	//or going down stairs.
	if (!collided(x + player_width / 2, y + height + 4))
	{
		y -= verticalSpeed / 3;
		verticalSpeed -= 1.0;
	}
	else
	{
		verticalSpeed = 0.0;
		//Correction for falling a bit too far...into a floor tile!
		while (collided(x + player_width / 2, y + height))
			y -= 2;
	}

	//Check for collisions with foreground tiles at "toe height".
	//Would be better to get a tile value and test it to eliminate that aspect.
	if (facingRight)
	{
		if (collided(x + player_width, y + height))
			x = oldpx;
	}
	else
	{
		if (collided(x + player_width / 10, y + height) || x < 0)
			x = oldpx;
	}

	//Check for collision with tile from below if he is rising
	//Also, check vertical movement so he does not "blow out the top",
	//which causes a system interrupt as the map tries to follow him.
	if (verticalSpeed > 0)
	{
		if (collided(x + 10, y) || collided(x + player_width - 10, y))
			verticalSpeed = 0;
		if (y < 0)
		{
			y = 0;
			verticalSpeed = 0;
		}
	}


}//END updatePlayer



//Determine if coordinates passed in are in contact with a tile
//as long as tile is collidable.
unsigned char Sprite::collided(float x, float y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return (blockdata->tl || blockdata->tr || blockdata->bl || blockdata->br);
}//END collided

//Draw the player's current image at map positions specified
void Playerclass::drawPlayer(int mapxoff, int mapyoff)
{

	if (collisionTimer < 1)
	{
		if (facingRight)
			al_draw_bitmap(player_image[curframe], (x - mapxoff), (y - mapyoff), 0);
		else
			al_draw_bitmap(player_image[curframe], (x - mapxoff), (y - mapyoff),
				ALLEGRO_FLIP_HORIZONTAL);
	}

	else if (collisionTimer < 140)
	{
		if (collisionTimer % 5 == 0)
		{
			if (facingRight)
				al_draw_bitmap(player_image[curframe], (x - mapxoff), (y - mapyoff), 0);
			else
				al_draw_bitmap(player_image[curframe], (x - mapxoff), (y - mapyoff),
					ALLEGRO_FLIP_HORIZONTAL);
		}
		collisionTimer--;
	}
	else
	{
		if (facingRight)
			al_draw_bitmap(playerHit, (x - mapxoff), (y - mapyoff), 0);
		else
			al_draw_bitmap(playerHit, (x - mapxoff), (y - mapyoff),
				ALLEGRO_FLIP_HORIZONTAL);
		collisionTimer--;
	}

}//END drawPlayer

