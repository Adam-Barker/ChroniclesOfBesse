//FILE : projectiles.h
//PROG : Adam Barker 
//PURP : Class for the player's sprite's projectiles

#include <iostream>
#include <vector>
#include "spriteclass.h"

class Projectile:Sprite
{
private:
	int proj_width, proj_height;
	std::vector<int> projectileX;
	std::vector<int> projectileY;
	std::vector<bool> facingRight;
	std::vector<ALLEGRO_BITMAP*> projectiles;
	ALLEGRO_BITMAP *projectile;
	ALLEGRO_SAMPLE *projectileSound;
	ALLEGRO_SAMPLE_ID projectileID;


public:
	Projectile();
	~Projectile();
	void updateProjectile(int, int, bool, bool);
	bool projectileCollide(int, int, int);
	void drawProjectile(int, int);
};