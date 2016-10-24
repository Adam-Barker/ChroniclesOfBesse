//FILE : enemyclass.h
//PROG : Adam Barker 
//PURP : Class for the enemy sprite

#include "spriteclass.h"

class Enemyclass : Sprite
{
	private:
		float ex, ey;
		float oldex, oldey, hitex, hitey;
		int enemy_width;
		int pCollisionTimer;
		bool pCollide;
		ALLEGRO_BITMAP *enemy_image[4];
		ALLEGRO_BITMAP *enemyHit;

	public:
		Enemyclass();
		~Enemyclass();
		void updateEnemy(bool, bool);
		int getenemyWidth() { return enemy_width; }
		int getex() { return ex; }
		int getey() { return ey; }
		void drawEnemy(int, int);
		void setex(float);
		bool spriteCollide(int, int, int);
};

