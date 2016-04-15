#include "pickup.h"

Pickup::Pickup(SDL_Renderer *renderer, string filePath, int pickup2, float x, float y)
{
	active = true;

	if(pickup2 == 0){

		string basePath = filePath + "gaugeFill.png";

		pickup = IMG_LoadTexture(renderer, basePath.c_str());

	}else if(pickup2 == 1){

		string basePath = filePath + "gaugeFill.png";

		pickup = IMG_LoadTexture(renderer, basePath.c_str());

	}else if(pickup2 == 2){

		string basePath = filePath + "gaugeFill.png";

		pickup = IMG_LoadTexture(renderer, basePath.c_str());

	}else if(pickup2 == 3){

			string basePath = filePath + "pickup2.png";

			pickup = IMG_LoadTexture(renderer, basePath.c_str());

		}

	pickupRect.x = x;
	pickupRect.y = y;

	int w, h;
	SDL_QueryTexture(pickup, NULL, NULL, &w, &h);
	pickupRect.w = w;
	pickupRect.h =h;

	posJ_X = pickupRect.x;
	posJ_Y = pickupRect.y;

}

void Pickup::Draw(SDL_Renderer * renderer)
{
	SDL_RenderCopy(renderer, pickup, NULL, &pickupRect);

}

void Pickup::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed * deltaTime);

	pickupRect.x = (int)(posJ_X + 0.5f);

}

void Pickup::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed * deltaTime);

	pickupRect.y = (int)(posJ_Y + 0.5f);

}

