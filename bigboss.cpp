#include "bigboss.h"

BigBoss::BigBoss(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	active = true;

	fire = Mix_LoadWAV((audioPath + "Laser2.wav").c_str());

	string basePath = filePath + "bigboss.png";

	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	baseRect.x = x;
	baseRect.y = y;

	int w, h;
	SDL_QueryTexture(tBase, NULL, NULL, &w, &h);
	baseRect.w = w;
	baseRect.h = h;

	barrelRect.x = x + 34;
	barrelRect.y = y + 34;

	SDL_QueryTexture(tBarrel, NULL, NULL, &w, &h);
	barrelRect.w = w;
	barrelRect.h = h;

	center.x = 12;
	center.y = 12;

	string bulletPath = filePath + "bossbullet.png";

	for(int i = 0; i < 10; i++)
	{
		TurretBullet tmpBullet(renderer, bulletPath, 1500, 1500);

		bulletList.push_back(tmpBullet);
	}

	srand(time_t(NULL));

	posB_X = baseRect.x;
	posB_Y = baseRect.y;

	posT_X = barrelRect.x;
	posT_Y = barrelRect.y;

	health = 100;

}

void BigBoss::RemoveHealthBullet()
{
	health -= 1;

	if(health <= 0){
		Reset();
	}
}

void BigBoss::RemoveHealthMissile()
{
	health -= 5;

	if(health <= 0){
		Reset();
	}
}

void BigBoss::RemoveHealthBeam(){

	health -= 10;

	if(health <= 0){
		Reset();
	}

}

void BigBoss::Reset()
{
	baseRect.x = -1000;

	barrelRect.x = -1000;

	posT_X = baseRect.x;

	posT_X = barrelRect.x;

	health = 100;

	active = false;
}

void BigBoss::ResetBoss()
{
	baseRect.x = 325.0;

	baseRect.y = 25.0;

	health = 100;

	active = true;
}

void BigBoss::TankMoveX(float tankSpeed, float deltaTime)
{
	posB_X += (tankSpeed) * deltaTime;
	posT_X += (tankSpeed) * deltaTime;

	baseRect.x = (int)(posB_X + 0.5f);
	barrelRect.x = (int)(posT_X + 0.5f);
}

void BigBoss::TankMoveY(float tankSpeed, float deltaTime)
{
	posB_Y += (tankSpeed) * deltaTime;
	posT_Y += (tankSpeed) * deltaTime;

	baseRect.y = (int)(posB_Y + 0.5f);
	barrelRect.y = (int)(posT_Y + 0.5f);
}

void BigBoss::Draw(SDL_Renderer *renderer)
{

	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active){

			bulletList[i].Draw(renderer);
		}
	}

	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

	SDL_RenderCopyEx(renderer, tBarrel, NULL, &barrelRect, turrentangle, &center, SDL_FLIP_NONE);
}

void BigBoss::Update(float deltaTime, SDL_Rect tankRect)
{
	x = (tankRect.x + (tankRect.w/2)) - (posRect.x + (baseRect.w/2));
	y = (tankRect.y + (tankRect.h/2)) - (posRect.y + (baseRect.h/2));
	turrentangle = atan2(y,x) * 180 / 3.14;

	if(SDL_GetTicks() > fireTime){

		if(baseRect.x > 0 && baseRect.x < 1024 && baseRect.y > 0 && baseRect.y < 768){
			CreateBullet(tankRect);
		}

		fireTime = SDL_GetTicks() + (rand() % 3+ 1) * 1000;
	}


	for(int i =0; i < bulletList.size(); i ++)
	{
		if(bulletList[i].active){

			bulletList[i].Update(deltaTime);
		}
	}

}

void BigBoss::CreateBullet(SDL_Rect target)
{

	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active == false){

			bulletList[i].Start(target, baseRect);

			Mix_PlayChannel(-1, fire, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = ((baseRect.x + (baseRect.w/2)) - (bulletList[i].posRect.w/2));
			bulletList[i].posRect.y = ((baseRect.y + (baseRect.h/2)) - (bulletList[i].posRect.h/2));

			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			break;
		}
	}

}
