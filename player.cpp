#include "player.h"

const int JOYSTICK_DEAD_ZONE = 8000;

Ship::Ship(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{
	active = true;

	playerNum = pNum;

	speed = 200.0f;

	fire = Mix_LoadWAV((audioPath + "Laser.wav").c_str());

	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 3;

	TTF_Init();

	font = TTF_OpenFont((audioPath + "Long_Shot.ttf").c_str(), 40);

	if(playerNum == 0){
		scorePos.x = scorePos.y = 10;
		livesPos.x = 10;
		livesPos.y = 60;
	}

	UpdateScore(renderer);

	UpdateLives(renderer);


	if(playerNum == 0){
		playerPath = filePath + "spaceship.png";
	}

	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	posRect.x = x;
	posRect.y = y;
	posRect.w = 144;
	posRect.h = 104;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	xDirOld = 1;
	yDirOld = 0;

	center.x = posRect.w/2;
	center.y = posRect.h/2;

	string bulletPath;

	if(playerNum == 0){
		bulletPath = filePath + "missile.png";
	}

}

void Ship::Reset() {
	if (playerNum == 0) {
		posRect.x = 250.0;
		posRect.y = 500.0;
	}

	pos_X = posRect.x;
	pos_Y = posRect.y;
	playerLives = 3;
	playerScore = 0;
	xDir = 0;
	yDir = 0;
	active = true;

}

void Ship::UpdateLives(SDL_Renderer *renderer){

	string Result;
	ostringstream convert;
	convert << playerLives;
	Result = convert.str();

	tempLives = "Player Lives: " + Result;

	if(playerNum == 0){
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);
	}

	livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);

	SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);

	SDL_FreeSurface(livesSurface);

	oldLives = playerLives;

}

void Ship::UpdateScore(SDL_Renderer *renderer){

	string Result;
	ostringstream convert;
	convert << playerScore;
	Result = convert.str();

	tempScore = "Player Score: " + Result;

	if(playerNum == 0){
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);
	}

	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	SDL_FreeSurface(scoreSurface);

	oldScore = playerScore;

}

void Ship::Update(float deltaTime, SDL_Renderer *renderer)
{
	if(Xvalue != 0 || Yvalue != 0){
		shipangle = atan2(Yvalue,Xvalue) * 180/3.14;

		oldAngle = shipangle;

		float radians = (shipangle * 3.14)/180;

		float move_x = speed *cos(radians);
		float move_y = speed *sin(radians);

		pos_X += (move_x) * deltaTime;
		pos_Y += (move_y) * deltaTime;

		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);

	}else {
		shipangle = oldAngle;
	}

	if(posRect.x < 0){
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w){
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y < 0){
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h){
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

		if(playerScore != oldScore){

			UpdateScore(renderer);

		}

		if(playerLives != oldScore){

			UpdateLives(renderer);

		}

}

void Ship::Draw(SDL_Renderer *renderer)
{

	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, shipangle, &center, SDL_FLIP_NONE);

	SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

	SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);
}


void Ship::OnControllerAxis(Sint16 X, Sint16 Y)
{
	Xvalue = X;
	Yvalue = Y;

	if(!(Xvalue < -JOYSTICK_DEAD_ZONE)&&!(Xvalue > JOYSTICK_DEAD_ZONE))
	{
		Xvalue = 0.0f;
	}

	if(!(Yvalue < -JOYSTICK_DEAD_ZONE)&&!(Yvalue > JOYSTICK_DEAD_ZONE))
	{
		Yvalue = 0.0f;
	}


}

