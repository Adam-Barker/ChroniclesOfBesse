//FILE : projectile.cpp
//PROG : Adam Barker 
//PURP : Implementations of the projectile objects

#include "projectile.h"
#include "scrollerDefs.h"


extern bool keys[];

Projectile::Projectile()
{
	proj_width = 32;
	proj_height = 32;
	height = 86;
	
	projectileSound = al_load_sample("projectile.wav");
	projectile = al_load_bitmap("corn.bmp");
	al_convert_mask_to_alpha(projectile, al_map_rgb(255, 0, 255));

}

Projectile::~Projectile()
{
		al_destroy_bitmap(projectile);
		al_destroy_sample(projectileSound);
}

void Projectile::updateProjectile(int pX, int pY, bool facingR, bool pCollide)
{
	if (keys[S])
	{
		projectiles.push_back(projectile);
		facingRight.push_back(facingR);
		projectileY.push_back(pY + proj_height / 2);
		projectileX.push_back(pX);
		al_play_sample(projectileSound, 0.7, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, &projectileID);
	}

	keys[S] = false;
	
	for (int ct = 0; ct < projectiles.size(); ct++)
	{
		if (facingRight[ct])
			projectileX[ct] += 4;
		else
			projectileX[ct] -= 4;
	
		if (facingRight[ct])
		{
			if (collided(projectileX[ct] + proj_width *1.50, projectileY[ct] + proj_height) || pCollide)
			{
				projectiles.erase(projectiles.begin() + ct);
				projectileX.erase(projectileX.begin() + ct);
				projectileY.erase(projectileY.begin() + ct);
				facingRight.erase(facingRight.begin() + ct);

				projectiles.shrink_to_fit();
				projectileX.shrink_to_fit();
				projectileY.shrink_to_fit();
				facingRight.shrink_to_fit();
			}
		}
		else
		{
			if (collided(projectileX[ct] + proj_width / 10, projectileY[ct] + proj_height)
				|| projectileX[ct] < 0 || pCollide)
			{
				projectiles.erase(projectiles.begin() + ct);
				projectileX.erase(projectileX.begin() + ct);
				projectileY.erase(projectileY.begin() + ct);
				facingRight.erase(facingRight.begin() + ct);

				projectiles.shrink_to_fit();
				projectileX.shrink_to_fit();
				projectileY.shrink_to_fit();
				facingRight.shrink_to_fit();
			}
		}
	}
}

bool Projectile::projectileCollide(int ex, int ey, int enemy_width)
{
	int collPWidth = proj_width * 0.80;
	int collEWidth = enemy_width * 0.80;
	int collPHeight = proj_height * 0.80;
	int collEHeight = height * 0.80;
	int collPXOffset = (proj_width - collPWidth) / 2.0;
	int collEXOffset = (enemy_width - collEWidth) / 2.0;
	int collPYOffset = (proj_height - collPHeight) / 2.0;
	int collEYOffset = (height - collEHeight) / 2.0;

	int left1, left2;
	int right1, right2;
	int top1, top2;
	int bottom1, bottom2;

	for (int ct = 0; ct < projectiles.size(); ct++)
	{
		left1 = projectileX[ct] + collPXOffset;
		left2 = ex + collEXOffset;
		right1 = left1 + collPWidth;
		right2 = left2 + collEWidth;
		top1 = projectileY[ct] + collPYOffset;
		top2 = ey + collEYOffset;
		bottom1 = top1 + collPHeight;
		bottom2 = top2 + collEHeight;

		if (bottom1 < top2) return false;
		if (top1 > bottom2) return false;
		if (right1 < left2) return false;
		if (left1 > right2) return false;
		return true;
	}
}

void Projectile::drawProjectile(int mapxoff, int mapyoff)
{
	for (int ct = 0; ct < projectiles.size(); ct++)
	{
		if (facingRight[ct])
			al_draw_bitmap(projectiles[ct], (projectileX[ct] - mapxoff + proj_width), (projectileY[ct] - mapyoff), 0);
		else
			al_draw_bitmap(projectiles[ct], (projectileX[ct] - mapxoff), (projectileY[ct] - mapyoff), 
				ALLEGRO_FLIP_HORIZONTAL);
	}
}
