#include <iostream>
#include <stdio.h>
#include <string>
#include <stdint.h>
#include <sstream>

using namespace std;

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "/final-game/";
string audio_dir = currentWorkingDirectory + "/final-game/";
#endif

#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <direct.h>
#define getcwd _getcwd

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "/final-game/";
string audio_dir = currentWorkingDirectory + "/final-game/";

#endif

#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include <unistd.h>

string currentWorkingDirectory(getcwd(NULL, 0));
string images_dir = currentWorkingDirectory + "/final-game/";
string audio_dir = currentWorkingDirectory + "/final-game/";

#endif

SDL_Rect bkgd1Pos, bkgd2Pos;

int bkgdSpeed = 100;

float b1pos_X = 0, b1pos_Y = 0;
float b2pos_X = 0, b2pos_Y = -768;

float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

void UpdateBackground(float deltaTime){

	b1pos_Y +=(bkgdSpeed *1)* deltaTime;
	bkgd1Pos.y = (int)(b1pos_Y + 0.5f);

	if(bkgd1Pos.y >= 768){
		bkgd1Pos.y = -768;
		b1pos_Y = bkgd1Pos.y;

	}

	b2pos_Y +=(bkgdSpeed *1)* deltaTime;
	bkgd2Pos.y = (int)(b2pos_Y + 0.5f);

	if(bkgd2Pos.y >= 768){
		bkgd2Pos.y = -768;
		b2pos_Y = bkgd2Pos.y;
	}
}

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//joystick direction vars
float xDir, yDir;

//cursor float vars for movement
float pos_X, pos_Y;

//create SDL Rectangle for the 2 player graphic
SDL_Rect mousePos, activePos;

//var from cursor speed
int mouseSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event) {

	if(event.which == 0){
		if(event.axis == 0) {

			if(event.value < - JOYSTICK_DEAD_ZONE){
				xDir = -1.0f;
			}else if (event.value > JOYSTICK_DEAD_ZONE){
				xDir = 1.0f;
			} else {
				xDir = 0.0f;
			}
		}

		if(event.axis == 1) {

				if(event.value < - JOYSTICK_DEAD_ZONE){
					yDir = -1.0f;
				}else if (event.value > JOYSTICK_DEAD_ZONE){
					yDir = 1.0f;
				} else {
					yDir = 0.0f;
				}
			}
		}
	}

void UpdateCursor(float deltaTime){

	pos_X += (mouseSpeed * xDir) * deltaTime;
	pos_Y += (mouseSpeed * yDir) * deltaTime;

	mousePos.x = (int) (pos_X + 0.5f);
	mousePos.y = (int) (pos_Y + 0.5f);

	activePos.x = mousePos.x;
	activePos.y = mousePos.y;

	if(mousePos.x < 0){
		mousePos.x = 0;
		pos_X = mousePos.x;
	}

	if(mousePos.x > 1024 - mousePos.w){
		mousePos.x = 1024 - mousePos.w;
		pos_X = mousePos.x;
	}

	if(mousePos.y < 0){
		mousePos.y = 0;
		pos_Y = mousePos.y;
	}

	if(mousePos.y > 768 - mousePos.h){
		mousePos.y = 768 - mousePos.h;
		pos_Y = mousePos.y;
	}
}

bool InstructionsOver = false, StartGameOver = false,
BackStoryOver = false, QuitOver = false, MenuOver = false;

int main(int argc, char* argv[]){

	srand(time_t(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = NULL;

	SDL_Renderer *renderer = NULL;

	window = SDL_CreateWindow("Space wars",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1024, 768,
			SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *surface = IMG_Load((images_dir + "background.png").c_str());

	SDL_Texture *bkgd1;

	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Texture *bkgd2;

	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	bkgd1Pos.x = 0;
	bkgd1Pos.y = 0;
	bkgd1Pos.w = 1024;
	bkgd1Pos.h = 768;

	bkgd2Pos.x = 0;
	bkgd2Pos.y = 768;
	bkgd2Pos.w = 1024;
	bkgd2Pos.h = 768;

	surface = IMG_Load((images_dir + "mouse.png").c_str());

	SDL_Texture *mouse;

	mouse = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	mousePos.x = 0;
	mousePos.y = 0;
	mousePos.w = 29;
	mousePos.h = 28;

	activePos.x = 10;
	activePos.y = 10;
	activePos.w = 10;
	activePos.h = 10;

	surface = IMG_Load((images_dir + "MainMenu.png").c_str());

	SDL_Texture *mainmenu;

	mainmenu = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect menuPos;

	menuPos.x = 350;
	menuPos.y = 75;
	menuPos.w = 331;
	menuPos.h = 64;

	surface = IMG_Load((images_dir + "StartGame.png").c_str());

	SDL_Texture *StartGame;

	StartGame = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "StartGameO.png").c_str());

	SDL_Texture *StartGameO;

	StartGameO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect StartGamePos;

	StartGamePos.x = 350;
	StartGamePos.y = 225;
	StartGamePos.w = 330;
	StartGamePos.h = 54;

	surface = IMG_Load((images_dir + "Instructions.png").c_str());

	SDL_Texture *Instructions;

	Instructions = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "InstructionsO.png").c_str());

	SDL_Texture *InstructionsO;

	InstructionsO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect InstructionsPos;

	InstructionsPos.x = 350;
	InstructionsPos.y = 375;
	InstructionsPos.w = 330;
	InstructionsPos.h = 54;

	surface = IMG_Load((images_dir + "BackStory.png").c_str());

	SDL_Texture *BackStory;

	BackStory = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "BackStoryO.png").c_str());

	SDL_Texture *BackStoryO;

	BackStoryO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect BackStoryPos;

	BackStoryPos.x = 375;
	BackStoryPos.y = 525;
	BackStoryPos.w = 285;
	BackStoryPos.h = 67;

	surface = IMG_Load((images_dir + "Quit.png").c_str());

	SDL_Texture *Quit;

	Quit = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "QuitO.png").c_str());

	SDL_Texture *QuitO;

	QuitO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect QuitPos;

	QuitPos.x = 435;
	QuitPos.y = 650;
	QuitPos.w = 126;
	QuitPos.h = 58;

	surface = IMG_Load((images_dir + "InsructionsText.png").c_str());

	SDL_Texture *InsructionsText;

	InsructionsText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect InsructionsTextPos;

	InsructionsTextPos.x = 0;
	InsructionsTextPos.y = 0;
	InsructionsTextPos.w = 1013;
	InsructionsTextPos.h = 431;

	surface = IMG_Load((images_dir + "Menubutton.png").c_str());

	SDL_Texture *Menubutton;

	Menubutton = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "MenubuttonO.png").c_str());

	SDL_Texture *MenubuttonO;

	MenubuttonO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect MenubuttonPos;

	MenubuttonPos.x = 100;
	MenubuttonPos.y = 700;
	MenubuttonPos.w = 106;
	MenubuttonPos.h = 32;

	SDL_GameController* gGameController = NULL;

	gGameController = SDL_GameControllerOpen(0);

	SDL_GameControllerEventState(SDL_ENABLE);

	SDL_Event e;

	enum GameState{MENU, INSTRUCTIONS, BACKSTORY, STARTGAME, WIN,LOSE};

	GameState gameState = MENU;

	bool menu, instructions, backstory, startgame, win, lose, quit;

	quit = false;

	while(!quit)
	{
		switch(gameState)
		{
		case MENU:
			menu = true;

			while(menu)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				if(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch(e.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(e.cdevice.which == 0)
						{
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(StartGameOver)
								{
									menu = false;
									gameState = STARTGAME;
									StartGameOver = false;
								}

								if(InstructionsOver)
								{
									menu = false;
									gameState = INSTRUCTIONS;
									InstructionsOver = false;
								}

								if(BackStoryOver)
								{
									menu = false;
									gameState = BACKSTORY;
									BackStoryOver = false;
								}

								if(QuitOver)
								{
									menu = false;
									quit = true;
									QuitOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
						break;
					}
				}

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				InstructionsOver = SDL_HasIntersection(&activePos, &InstructionsPos);
				BackStoryOver = SDL_HasIntersection(&activePos, &BackStoryPos);
				StartGameOver = SDL_HasIntersection(&activePos, &StartGamePos);
				QuitOver = SDL_HasIntersection(&activePos, &QuitPos);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				if(StartGameOver)
				{
					SDL_RenderCopy(renderer, StartGameO, NULL, &StartGamePos);
				}
				else
				{
					SDL_RenderCopy(renderer, StartGame, NULL, &StartGamePos);
				}

				if(InstructionsOver)
				{
					SDL_RenderCopy(renderer, InstructionsO, NULL, &InstructionsPos);
				}
				else
				{
					SDL_RenderCopy(renderer, Instructions, NULL, &InstructionsPos);
				}

				if(BackStoryOver)
				{
					SDL_RenderCopy(renderer, BackStoryO, NULL, &BackStoryPos);
				}
				else
				{
					SDL_RenderCopy(renderer, BackStory, NULL, &BackStoryPos);
				}

				if(QuitOver)
				{
					SDL_RenderCopy(renderer, QuitO, NULL, &QuitPos);
				}
				else
				{
					SDL_RenderCopy(renderer, Quit, NULL, &QuitPos);
				}


				SDL_RenderCopy(renderer, mouse, NULL, &mousePos);

				SDL_RenderPresent(renderer);
			}
			break;

		case STARTGAME:
			startgame = true;

			while(startgame)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				if(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch(e.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(e.cdevice.which == 0)
						{
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
						break;
					}
				}

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderPresent(renderer);
			}
			break;

		case INSTRUCTIONS:
			instructions = true;

			while(instructions)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				if(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch(e.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(e.cdevice.which == 0)
						{
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(MenuOver)
								{
									instructions = false;
									gameState = MENU;
									StartGameOver = false;
								}

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
						break;
					}
				}

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				MenuOver = SDL_HasIntersection(&activePos, &MenubuttonPos);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderCopy(renderer, InsructionsText, NULL, &InsructionsTextPos);

				if(MenuOver)
				{
					SDL_RenderCopy(renderer, MenubuttonO, NULL, &MenubuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, Menubutton, NULL, &MenubuttonPos);
				}


				SDL_RenderCopy(renderer, mouse, NULL, &mousePos);

				SDL_RenderPresent(renderer);
			}
			break;

		case BACKSTORY:
			backstory = true;

			while(backstory)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				if(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch(e.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(e.cdevice.which == 0)
						{
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
						break;
					}
				}

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderPresent(renderer);
			}
			break;

		case WIN:
			win = true;

			while(win)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				if(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch(e.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(e.cdevice.which == 0)
						{
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
						break;
					}
				}

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderPresent(renderer);
			}
			break;

		case LOSE:
			lose = true;

			while(lose)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				if(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch(e.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(e.cdevice.which == 0)
						{
							if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(e.caxis);
						break;
					}
				}

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderPresent(renderer);
			}
			break;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;

}
