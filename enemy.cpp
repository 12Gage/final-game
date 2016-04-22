#include "enemy.h"

Enemy::Enemy(SDL_Renderer *renderer, string filePath)
{
	active = false;

	string enemyPath = filePath + "Aenemy.png";

	SDL_Surface *surface = IMG_Load(enemyPath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int h, w;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	Reset();

	xDir = 0;
	yDir = 1;

	angle = rand() % (360);

	center.x = posRect.w/2;
	center.y = posRect.h/2;

}

void Enemy::Reset(){

	speed = rand() % (5) + 5;
	speed *= 100;

	posRect.x = rand() % (1024-posRect.w) + 1;
	posRect.y = - posRect.h;

	pos_X = posRect.x;
	pos_Y = posRect.y;
}

void Enemy::enemyMoveX(float playerSpeed, float deltaTime)
{
	posT_X += (playerSpeed) *deltaTime;

	posRect.x = (int)(posT_X + 0.5f);
}

void Enemy::enemyMoveY(float playerSpeed, float deltaTime)
{
	posT_Y += (playerSpeed) *deltaTime;

	posRect.y = (int)(posT_Y + 0.5f);

}

void Enemy::Update(float deltaTime, SDL_Rect playerRect)
{
	pos_Y += (speed *yDir) * deltaTime;

	posRect.y = (int) (pos_Y + 0.5f);

	if(posRect.y > 768){

		Reset();
	}

	angle += .1;

	double distancex = (posRect.x + playerRect.x) * (posRect.x + playerRect.x);
	double distancey = (posRect.y + playerRect.y) * (posRect.y + playerRect.y);

	double calcdistance = sqrt(distancex + distancey);

	if(calcdistance <= 250){
		active = true;
	}else{
		active = false;
	}

	if(active){
		x = (playerRect.x + (playerRect.w/2)) - (posRect.x + (posRect.w/2));
		y = (playerRect.y + (playerRect.h/2)) - (posRect.y + (posRect.h/2));
		tankangle = atan2(y,x) * 180/3.14;

		float radians = (tankangle * 3.14)/180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		posT_X += (move_x) * deltaTime;
		posT_Y += (move_y) * deltaTime;

		posRect.x = (int)(posT_X + 0.5f);
		posRect.y = (int)(posT_Y + 0.5f);
	}

}

void Enemy::Draw(SDL_Renderer * renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, angle, &center, SDL_FLIP_NONE);
}

Enemy::~Enemy()
{

}
