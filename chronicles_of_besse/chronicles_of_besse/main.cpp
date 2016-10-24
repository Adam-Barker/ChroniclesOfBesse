//FILE : main.cpp
//PROG : Adam Barker (Prototype: Dave Jones)
//PURP : Loads maps, background music and controls game loop for drawing and rendering.

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//Include basic definitions/prototypes for mappy
#include "mappy_A5.h"

//One prototype and header files
int collided(int, int);
#include "scrollerDefs.h"
#include "spriteclass.h"
#include "playerclass.h"
#include "enemyclass.h"
#include "heart.h"
#include "projectile.h"
#include "cow.h"


//Global array for the keyboard
bool keys[] = { false, false, false, false, false, false };
bool eCollide = false;
bool pCollide = false;
const int eCount = 6;
int levelCt = 1;

int main(void)
{

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_SAMPLE *music = NULL;
	ALLEGRO_SAMPLE_ID musicID;

	bool redrawRequired = true, levelscrn = true, nextLevel = false, quit = false, keyWasPressed = false, start = false, end = false;
	int mapxoff, mapyoff;

	//Initialize allegro, etc
	if (!al_init())
	{
		fprintf(stderr, "Failed to init allegro!\n");
		return -1;
	}

	//Initialize keyboard input
	if (!al_install_keyboard())
	{
		fprintf(stderr, "Failed to init keyboard!\n");
		return -1;
	}

	//Initialize timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		fprintf(stderr, "Failed to init timer!\n");
		return -1;
	}

	//Initialize display
	display = al_create_display(SCRN_W, SCRN_H);
	if (!display)
	{
		fprintf(stderr, "Failed to init display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//Initialize the image mechanism
	al_init_image_addon();

	if (!al_install_audio()) return -9;
	if (!al_init_acodec_addon()) return -9;
	if (!al_reserve_samples(4)) return -9;

	music = al_load_sample("levelOne.wav");

	//Load the map using MappyAl function
	if (MapLoad("level_one.fmp", 1) != 0)
	{
		fprintf(stderr, "Failed to load the Mappy map.\n");
		return -7;
	}

	//Declare/instantiate the player sprite
	Playerclass *player = new Playerclass(mapheight, mapblockheight, 20);
	Playerclass *player2 = new Playerclass(mapheight, mapblockheight, 30);
	Enemyclass *enemies[eCount];
	Heart *heart = new Heart;
	Projectile *projectile = new Projectile;
	Cow *cow;


	for (int ct = 0; ct < eCount; ++ct)
	{
		enemies[ct] = new Enemyclass();
		if (ct > 0)
			enemies[ct]->setex(ct * 8 * mapblockwidth);
		else
			enemies[ct]->setex((mapwidth - 1) * mapblockwidth - enemies[ct]->getenemyWidth());
	}

	//Set the back buffer as the drawing bitmap for the display
	al_set_target_bitmap(al_get_backbuffer(display));

	//Initialize the event queue
	eventQueue = al_create_event_queue();
	if (!eventQueue)
	{
		fprintf(stderr, "Failed to create event queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	//Register events as arriving from these sources:  display, timer, keyboard
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	al_start_timer(timer);

	al_play_sample(music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicID);

	//Central game loop
	while (!quit)
	{
		ALLEGRO_EVENT evt;
		//Wait for one of the allegro-defined events
		al_wait_for_event(eventQueue, &evt);

		//An event was generated. Check for all possible event types
		switch (evt.type)
		{
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (evt.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				quit = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACEBAR] = true;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = true;			
				break;
			case ALLEGRO_KEY_ENTER:
				start = true;
				break;
			}
			break;

		case ALLEGRO_EVENT_KEY_UP:
			switch (evt.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				quit = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACEBAR] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = false;
				break;
			}
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			quit = true;
			break;

		case ALLEGRO_EVENT_TIMER:

			if (start)
			{

				if (heart->getQuit())
				{
					al_draw_bitmap(al_load_bitmap("gameover.bmp"), 0, 0, 0);
					al_flip_display();
					al_rest(10);
					quit = true;
				}

				//Update the player's movement & collision status
				for (int ct = 0; ct < eCount; ++ct)
				{
					pCollide = false;

					if (projectile->projectileCollide(enemies[ct]->getex(), enemies[ct]->getey(),
						enemies[ct]->getenemyWidth()))
					{
						pCollide = true;
						projectile->updateProjectile(player->getX(), player->getY(), player->getFacing(), pCollide);
						enemies[ct]->updateEnemy(eCollide, pCollide);
					}
					else if (enemies[ct]->spriteCollide(player->getPlayerWidth(), player->getX(), player->getY()))
					{
						eCollide = true;
						enemies[ct]->updateEnemy(eCollide, pCollide);
						heart->updateHeart(SCRN_W - 175, eCollide);
					}
					else
						enemies[ct]->updateEnemy(eCollide, pCollide);
				}
				player->updatePlayer(eCollide);
				player2->updatePlayer(eCollide);

				eCollide = false;

				heart->updateHeart(SCRN_W - 175, eCollide);
				projectile->updateProjectile(player->getX(), player->getY(), player->getFacing(), pCollide);

				if (levelCt == 3)
					cow->updateCow();

				//update the map scroll position
				mapxoff = player->getX() + player->getPlayerWidth() / 2 - SCRN_W / 2 + 10;
				mapyoff = player->getY() + player->getHeight() / 2 - SCRN_H / 2 + 10;

				//Avoid moving beyond the map edge
				if (mapxoff < 0) mapxoff = 0;
				if (mapxoff > (mapwidth * mapblockwidth - SCRN_W))
					mapxoff = mapwidth * mapblockwidth - SCRN_W;
				if (mapyoff < 0)
					mapyoff = 0;
				if (mapyoff > (mapheight * mapblockheight - SCRN_H))
					mapyoff = mapheight * mapblockheight - SCRN_H;

				redrawRequired = true;
				break;
			}

		}//END switch evt.type

		if (redrawRequired && al_is_event_queue_empty(eventQueue) && !end)
		{
			redrawRequired = false;

			if (!start)
			{
				al_draw_bitmap(al_load_bitmap("home.bmp"), 0, 0, 0);
				al_flip_display();
			}
			else
			{
				if (levelscrn) {
					levelscrn = false;
					if (levelCt == 1)
					{
						al_draw_bitmap(al_load_bitmap("levelOne.bmp"), 0, 0, 0);
						al_flip_display();
						al_rest(5);
					}
					else if (levelCt == 2)
					{
						al_stop_sample(&musicID);
						music = al_load_sample("levelTwo.wav");
						al_draw_bitmap(al_load_bitmap("levelTwo.bmp"), 0, 0, 0);
						al_flip_display();
						al_rest(5);
						al_play_sample(music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicID);
					}
					else
					{
						al_stop_sample(&musicID);
						music = al_load_sample("levelThree.wav");
						al_draw_bitmap(al_load_bitmap("levelThree.bmp"), 0, 0, 0);
						al_flip_display();
						al_rest(5);
						al_play_sample(music, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicID);
					}
				}

				//Draw the background & foreground tiles
				MapDrawBG(mapxoff, mapyoff, 0, 0, SCRN_W - 1, SCRN_H);
				MapDrawFG(mapxoff, mapyoff, 0, 0, SCRN_W - 1, SCRN_H, 0);

				//Draw the player's sprite
				projectile->drawProjectile(mapxoff, mapyoff);

				if (levelCt == 3)
				{
					cow->drawCow(mapxoff, mapyoff);
					if (cow->cowCollide(player->getPlayerWidth(), player->getX(), player->getY()))
					{
						end = true;
					}
				}

				for (int ct = 0; ct < eCount; ++ct)
				{
					enemies[ct]->drawEnemy(mapxoff, mapyoff);
				}

				player->drawPlayer(mapxoff, mapyoff);
				player2->drawPlayer(mapxoff, mapyoff);

				heart->drawHeart();

				if (mapwidth * mapblockwidth - (player->getX() + 30) < 0)
				{
					levelCt++;
					nextLevel = true;
					levelscrn = true;
				}

				al_flip_display();
			}

			if (nextLevel)
			{
				nextLevel = false;

				MapFreeMem();

				delete player;
				delete projectile;
				delete heart;
				for (int ct = 0; ct < eCount; ct++)
					delete enemies[ct];

				if (levelCt == 2)
				{
					if (MapLoad("level_two.fmp", 1) != 0)
					{
						fprintf(stderr, "Failed to load the Mappy map.\n");
						return -7;
					}
				}
				else
				{
					if (MapLoad("level_three.fmp", 1) != 0)
					{
						fprintf(stderr, "Failed to load the Mappy map.\n");
						return -7;
					}
					cow = new Cow(mapblockwidth, mapblockheight, mapwidth);
				}

				player = new Playerclass(mapheight, mapblockheight, 20);
				projectile = new Projectile();
				heart = new Heart();

				for (int ct = 0; ct < eCount; ct++)
				{
					enemies[ct] = new Enemyclass();
					if (ct > 0)
						enemies[ct]->setex(ct * 8 * mapblockwidth);
					else
						enemies[ct]->setex((mapwidth - 1) * mapblockwidth - enemies[ct]->getenemyWidth());
				}

				//Set the back buffer as the drawing bitmap for the display
				al_set_target_bitmap(al_get_backbuffer(display));
			}
		}
		else if (end)
		{
			al_stop_sample(&musicID);
			music = al_load_sample("end.wav");
			al_play_sample(music, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musicID);

			al_draw_bitmap(al_load_bitmap("end.bmp"), 0, 0, 0);
			al_flip_display();
			al_rest(10);
			quit = true;
		}
	}//END while

	delete player;
	for (int ct = 0; ct < eCount; ++ct)
		delete enemies[ct];
	delete heart;
	delete projectile;
	if (levelCt == 3)
		delete cow;
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(eventQueue);
	al_destroy_sample(music);
	//Free memory allocated to the Mappy map
	MapFreeMem();

	return 0;
}//END main


int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}//END collided