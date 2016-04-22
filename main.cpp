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

#include "player.h"
#include "enemy.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "turrent.h"
#include "pickup.h"
#include "explode.h"
#include "bigboss.h"

vector<Enemy> enemyList;

vector<Explode> explodeList;

void MakeExplosion(int x, int y){

	for(int i = 0; i < explodeList.size(); i ++)
	{
		if(explodeList[i].active == false){

			explodeList [i].active = true;

			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;

			break;
		}
	}
}

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

const int JOYSTICK_DEAD_ZONE = 8000;

float xDir, yDir;

float pos_X, pos_Y;

SDL_Rect mousePos, activePos;

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

bool InstructionsOver = false, instructionsover = false, StartGameOver = false, startgameover = false,
BackStoryOver = false, backstoryover = false, QuitOver = false, MenuOver = false;

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

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());

	if (!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "Explosion.wav").c_str());

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

	surface = IMG_Load((images_dir + "InstructionsText.png").c_str());

	SDL_Texture *InsructionsText;

	InsructionsText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect InsructionsTextPos;

	InsructionsTextPos.x = 0;
	InsructionsTextPos.y = 200;
	InsructionsTextPos.w = 1013;
	InsructionsTextPos.h = 431;

	surface = IMG_Load((images_dir + "BackStoryText.png").c_str());

	SDL_Texture *BackStoryText;

	BackStoryText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect BackStoryTextPos;

	BackStoryTextPos.x = 250;
	BackStoryTextPos.y = 200;
	BackStoryTextPos.w = 558;
	BackStoryTextPos.h = 345;

	surface = IMG_Load((images_dir + "WinText.png").c_str());

	SDL_Texture *WinText;

	WinText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect WinTextPos;

	WinTextPos.x = 350;
	WinTextPos.y = 325;
	WinTextPos.w = 357;
	WinTextPos.h = 95;

	surface = IMG_Load((images_dir + "LoseText.png").c_str());

	SDL_Texture *LoseText;

	LoseText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect LoseTextPos;

	LoseTextPos.x = 350;
	LoseTextPos.y = 300;
	LoseTextPos.w = 362;
	LoseTextPos.h = 144;

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

	surface = IMG_Load((images_dir + "BackStorybutton.png").c_str());

	SDL_Texture *BackStorybutton;

	BackStorybutton = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "BackStorybuttonO.png").c_str());

	SDL_Texture *BackStorybuttonO;

	BackStorybuttonO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect BackStorybuttonPos;

	BackStorybuttonPos.x = 400;
	BackStorybuttonPos.y = 700;
	BackStorybuttonPos.w = 186;
	BackStorybuttonPos.h = 43;

	surface = IMG_Load((images_dir + "InstructionsButton.png").c_str());

	SDL_Texture *InstructionsButton;

	InstructionsButton = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "InstructionsButtonO.png").c_str());

	SDL_Texture *InstructionsButtonO;

	InstructionsButtonO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect InstructionsButtonPos;

	InstructionsButtonPos.x = 400;
	InstructionsButtonPos.y = 700;
	InstructionsButtonPos.w = 221;
	InstructionsButtonPos.h = 32;

	surface = IMG_Load((images_dir + "StartGamebutton.png").c_str());

	SDL_Texture *StartGamebutton;

	StartGamebutton = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((images_dir + "StartGamebuttonO.png").c_str());

	SDL_Texture *StartGamebuttonO;

	StartGamebuttonO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect StartGamebuttonPos;

	StartGamebuttonPos.x = 750;
	StartGamebuttonPos.y = 700;
	StartGamebuttonPos.w = 212;
	StartGamebuttonPos.h = 32;

	SDL_Texture *gaugeBlank = IMG_LoadTexture(renderer, (images_dir + "gaugeBlank.png").c_str());
	SDL_Rect gaugeBlankRect;
	gaugeBlankRect.x = 370;
	gaugeBlankRect.y = 10;
	gaugeBlankRect.w = 19;
	gaugeBlankRect.h = 9;

	SDL_Texture *gaugeFill1 = IMG_LoadTexture(renderer, (images_dir + "gaugeFill1.png").c_str());
	SDL_Rect gaugeFill1Pos;
	gaugeFill1Pos.x = 370;
	gaugeFill1Pos.y = 10;
	gaugeFill1Pos.w = 19;
	gaugeFill1Pos.h = 9;

	SDL_Texture *gaugeFill2 = IMG_LoadTexture(renderer, (images_dir + "gaugeFill2.png").c_str());
	SDL_Rect gaugeFill2Pos;
	gaugeFill2Pos.x = 370;
	gaugeFill2Pos.y = 10;
	gaugeFill2Pos.w = 19;
	gaugeFill2Pos.h = 9;

	SDL_Texture *gaugeFill3 = IMG_LoadTexture(renderer, (images_dir + "gaugeFill3.png").c_str());
	SDL_Rect gaugeFill3Pos;
	gaugeFill3Pos.x = 370;
	gaugeFill3Pos.y = 10;
	gaugeFill3Pos.w = 19;
	gaugeFill3Pos.h = 9;

	bool havegaugeFill1 = false;
	bool havegaugeFill2 = false;
	bool havegaugeFill3 = false;

	Pickup gauge1 = Pickup(renderer, images_dir.c_str(), 0,200.0f,500.0f);
	Pickup gauge2 = Pickup(renderer, images_dir.c_str(), 1,200.0f,600.0f);
	Pickup gauge3 = Pickup(renderer, images_dir.c_str(), 2,200.0f,700.0f);

	Pickup missilePickup = Pickup(renderer, images_dir.c_str(), 3,600.0f,600.0f);

	SDL_GameController* gGameController = NULL;

	gGameController = SDL_GameControllerOpen(0);

	SDL_GameControllerEventState(SDL_ENABLE);

	SDL_Event e;

	Player player1 = Player(renderer, 0, images_dir.c_str(), audio_dir.c_str(), 500.0,500.0);

	Turrent turret1 = Turrent(renderer, images_dir.c_str(), audio_dir.c_str(), 10.0, 200.0);
	Turrent turret2 = Turrent(renderer, images_dir.c_str(), audio_dir.c_str(), 900.0, 200.0);

	BigBoss boss = BigBoss(renderer, images_dir.c_str(), audio_dir.c_str(), 325.0, 25.0);

	for (int i = 0; i < 20; i++)
	{
		Explode tmpExplode(renderer, images_dir, -1000, -1000);

		explodeList.push_back(tmpExplode);
	}

	enum GameState{MENU, INSTRUCTIONS, BACKSTORY, STARTGAME, LEVEL2, WIN,LOSE};

	GameState gameState = MENU;

	bool menu, instructions, backstory, startgame, level2, win, lose, quit;

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

			enemyList.clear();

			player1.Reset();

			turret1.ResetTurret1();

			turret2.ResetTurret2();

			gauge1.active = true;

			for(int i = 0; i < player1.bulletList.size(); i++)
			{
				player1.bulletList[i].Reset();
			}

			for (int i = 0; i < player1.missileList.size(); i++)
			{
				player1.missileList[i].Reset();
			}

			for (int i = 0; i < player1.beamList.size(); i++)
			{
				player1.beamList[i].Reset();
			}

			for(int i = 0; i < turret1.bulletList.size(); i++)
			{
				turret1.bulletList[i].Reset();
			}

			for(int i = 0; i < turret2.bulletList.size(); i++)
			{
				turret2.bulletList[i].Reset();
			}

			startgame = true;

			for(int i = 0; i < 3; i++)
			{
				Enemy tmpEnemy(renderer, images_dir);

				enemyList.push_back(tmpEnemy);
			}

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
							if(player1.active)
							{
								player1.OnControllerButton(e.cbutton);
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						if(player1.active)
						{
							player1.OnControllerAxis(e.caxis);
						}
						break;
					}
				}

				UpdateBackground(deltaTime);

				if(player1.active)
				{
					player1.Update(deltaTime, renderer);
				}

				turret1.Update(deltaTime, player1.posRect);

				turret2.Update(deltaTime, player1.posRect);

				if(player1.active == true)
				{
					for(int i = 0; i < enemyList.size(); i++)
					{
						enemyList[i].Update(deltaTime, player1.posRect);
					}

					for(int i = 0; i < player1.bulletList.size(); i++)
					{
						if(player1.bulletList[i].active == true)
						{
							for(int j = 0; j < enemyList.size(); j ++)
							{
								if(SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									enemyList[j].Reset();

									player1.bulletList[i].Reset();

									player1.playerScore +=100;
								}

								if(SDL_HasIntersection(&turret1.baseRect, &player1.bulletList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.bulletList[i].Reset();
									if(turret1.active == true)
									{
										turret1.RemoveHealthBullet();
									}
								}

								if(SDL_HasIntersection(&turret2.baseRect, &player1.bulletList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.bulletList[i].Reset();
									if(turret2.active == true)
									{
										turret2.RemoveHealthBullet();
									}
								}
							}
						}
					}

					for(int i = 0; i < player1.missileList.size(); i++)
					{
						if(player1.missileList[i].active == true)
						{
							for(int j = 0; j < enemyList.size(); j ++)
							{
								if(SDL_HasIntersection(&player1.missileList[i].posRect, &enemyList[j].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									enemyList[j].Reset();

									player1.missileList[i].Reset();

									player1.playerScore +=100;
								}

								if(SDL_HasIntersection(&turret1.baseRect, &player1.missileList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.missileList[i].Reset();

									if(turret1.active == true)
									{
										turret1.RemoveHealthMissile();
									}
								}

								if(SDL_HasIntersection(&turret2.baseRect, &player1.missileList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.missileList[i].Reset();

									if(turret2.active == true)
									{
										turret2.RemoveHealthMissile();
									}
								}
							}
						}
					}

					for(int i = 0; i < player1.beamList.size(); i++)
					{
						if(player1.beamList[i].active == true)
						{
							for(int j = 0; j < enemyList.size(); j ++)
							{
								if(SDL_HasIntersection(&player1.beamList[i].posRect, &enemyList[j].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									enemyList[j].Reset();

									player1.beamList[i].Reset();

									player1.playerScore +=100;
								}

								if(SDL_HasIntersection(&turret1.baseRect, &player1.beamList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.beamList[i].Reset();

									if(turret1.active == true)
									{
										turret1.RemoveHealthBeam();
									}
								}

								if(SDL_HasIntersection(&turret2.baseRect, &player1.beamList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.missileList[i].Reset();

									if(turret2.active == true)
									{
										turret2.RemoveHealthBeam();
									}
								}
							}
						}
					}


					for(int i = 0; i < enemyList.size(); i++)
					{
						if(SDL_HasIntersection(&player1.posRect,
								&enemyList[i].posRect))
						{
							Mix_PlayChannel(-1, explosionSound, 0);

							MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

							enemyList[i].Reset();

							player1.playerLives -= 1;

							if(player1.playerLives <= 0)
							{
								startgame = false;
								gameState = LOSE;
								break;
							}
						}
					}
				}

				for (int i = 0; i < turret1.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player1.posRect, &turret1.bulletList[i].posRect)){
						turret1.bulletList[i].Reset();

						player1.playerLives -= 1;

						Mix_PlayChannel(-1, explosionSound, 0);

						MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

						if(player1.playerLives <= 0)
						{
							startgame = false;
							gameState = LOSE;
							break;
						}
					}
				}

				for (int i = 0; i < turret2.bulletList.size(); i++)
				{
					if(SDL_HasIntersection(&player1.posRect, &turret2.bulletList[i].posRect)){
						turret2.bulletList[i].Reset();

						player1.playerLives -= 1;

						Mix_PlayChannel(-1, explosionSound, 0);

						MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

						if(player1.playerLives <= 0)
						{
							startgame = false;
							gameState = LOSE;
							break;
						}
					}
				}

				if(SDL_HasIntersection(&player1.posRect, &gauge1.pickupRect)){
					gauge1.active = false;
					havegaugeFill1 = true;
					gauge1.pickupRect.x = -5000;
					gauge1.pickupRect.y = -5000;
				}

				if(SDL_HasIntersection(&player1.posRect, &gauge2.pickupRect)){
					havegaugeFill2 = true;
					gauge2.active = false;
					gauge2.pickupRect.x = -5000;
					gauge2.pickupRect.y = -5000;
				}

				if(SDL_HasIntersection(&player1.posRect, &gauge3.pickupRect)){
					havegaugeFill3 = true;
					gauge3.active = false;
					gauge3.pickupRect.x = -5000;
					gauge3.pickupRect.y = -5000;
				}

				if(SDL_HasIntersection(&player1.posRect, &missilePickup.pickupRect)){
					missilePickup.pickupRect.x = -5000;
					missilePickup.pickupRect.y = -5000;
					player1.missiles = 3;
				}

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				for(int i = 0; i <enemyList.size(); i++)
				{
					enemyList[i].Draw(renderer);
				}

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				player1.Draw(renderer);

				turret1.Draw(renderer);

				turret2.Draw(renderer);

				if(player1.playerScore >= 100)
				{
					gauge1.pickupRect.x = 200;
					gauge1.pickupRect.y = 500;
					
					if (havegaugeFill1)
						SDL_RenderCopy(renderer, gaugeFill1, NULL, &gaugeFill1Pos);

					if(gauge1.active)
						gauge1.Draw(renderer);
				}

				if(player1.playerScore >= 200)
				{

					if(havegaugeFill2)
					SDL_RenderCopy(renderer, gaugeFill2, NULL, &gaugeFill2Pos);
					
					if(gauge2.active)
						gauge2.Draw(renderer);
				}

				if(player1.playerScore >= 300)
				{
					if(havegaugeFill3)
					SDL_RenderCopy(renderer, gaugeFill3, NULL, &gaugeFill3Pos);
					
					if(gauge3.active)
						gauge3.Draw(renderer);
				}

				if(player1.missiles <= 0)
				{
					missilePickup.pickupRect.x = 600;
					missilePickup.pickupRect.y = 600;
					
					if(missilePickup.active)
						missilePickup.Draw(renderer);
				}

				if (player1.playerScore >= 1000)
				{
					startgame = false;
					gameState = LEVEL2;
					break;
				}

				SDL_RenderPresent(renderer);
			}
			break;

		case LEVEL2:

			turret1.ResetTurret1();

			turret2.ResetTurret2();


			for (int i = 0; i < player1.bulletList.size(); i++)
			{
				player1.bulletList[i].Reset();
			}

			for (int i = 0; i < player1.missileList.size(); i++)
			{
				player1.missileList[i].Reset();
			}

			for (int i = 0; i < player1.beamList.size(); i++)
			{
				player1.beamList[i].Reset();
			}

			for(int i = 0; i < turret1.bulletList.size(); i++)
			{
				turret1.bulletList[i].Reset();
			}

			for(int i = 0; i < turret2.bulletList.size(); i++)
			{
				turret2.bulletList[i].Reset();
			}

			level2 = true;

			while (level2)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&e))
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch (e.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (e.cdevice.which == 0)
						{
							if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

							}
							if (player1.active)
							{
								player1.OnControllerButton(e.cbutton);
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						if (player1.active)
						{
							player1.OnControllerAxis(e.caxis);
						}
						break;
					}
				}

				UpdateBackground(deltaTime);

				if (player1.active)
				{
					player1.Update(deltaTime, renderer);
				}

				turret1.Update(deltaTime, player1.posRect);

				turret2.Update(deltaTime, player1.posRect);

				boss.Update(deltaTime, player1.posRect);

				if (player1.active == true)
				{
					for (int i = 0; i < player1.bulletList.size(); i++)
					{
						if (player1.bulletList[i].active == true)
						{
							for (int j = 0; j < enemyList.size(); j++)
							{

								if (SDL_HasIntersection(&turret1.baseRect, &player1.bulletList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.bulletList[i].Reset();

									if (turret1.active == true)
									{
										turret1.RemoveHealthBullet();
									}
								}

								if (SDL_HasIntersection(&turret2.baseRect, &player1.bulletList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.bulletList[i].Reset();

									if (turret2.active == true)
									{
										turret2.RemoveHealthBullet();
									}
								}

								if (SDL_HasIntersection(&boss.baseRect, &player1.bulletList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.bulletList[i].Reset();

									if (boss.active == true)
									{
										boss.RemoveHealthBullet();
									}
								}
							}
						}
					}

					for (int i = 0; i < player1.missileList.size(); i++)
					{
						if (player1.missileList[i].active == true)
						{
							for (int j = 0; j < enemyList.size(); j++)
							{
								if (SDL_HasIntersection(&turret1.baseRect, &player1.missileList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.missileList[i].Reset();

									if (turret1.active == true)
									{
										turret1.RemoveHealthMissile();
									}
								}

								if (SDL_HasIntersection(&turret2.baseRect, &player1.missileList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.missileList[i].Reset();

									if (turret2.active == true)
									{
										turret2.RemoveHealthMissile();
									}
								}

								if (SDL_HasIntersection(&boss.baseRect, &player1.missileList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.missileList[i].Reset();

									if (boss.active == true)
									{
										boss.RemoveHealthMissile();
									}
								}
							}
						}
					}

					for (int i = 0; i < player1.beamList.size(); i++)
					{
						if (player1.beamList[i].active == true)
						{
							for (int j = 0; j < enemyList.size(); j++)
							{
								if (SDL_HasIntersection(&turret1.baseRect, &player1.beamList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.beamList[i].Reset();

									if (turret1.active == true)
									{
										turret1.RemoveHealthBeam();
									}
								}

								if (SDL_HasIntersection(&turret2.baseRect, &player1.beamList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.beamList[i].Reset();

									if (turret2.active == true)
									{
										turret2.RemoveHealthBeam();
									}

								}

								if (SDL_HasIntersection(&boss.baseRect, &player1.beamList[i].posRect))
								{
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									player1.beamList[i].Reset();

									if (boss.active == true)
									{
										boss.RemoveHealthBeam();
									}
								}
							}
						}
					}
				}

				for (int i = 0; i < turret1.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player1.posRect, &turret1.bulletList[i].posRect)) {
						turret1.bulletList[i].Reset();

						player1.playerLives -= 1;

						Mix_PlayChannel(-1, explosionSound, 0);

						MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

						if (player1.playerLives <= 0)
						{
							level2 = false;
							gameState = LOSE;
							break;
						}
					}
				}

				for (int i = 0; i < turret2.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player1.posRect, &turret2.bulletList[i].posRect)) {
						turret2.bulletList[i].Reset();

						player1.playerLives -= 1;

						Mix_PlayChannel(-1, explosionSound, 0);

						MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

						if (player1.playerLives <= 0)
						{
							level2 = false;
							gameState = LOSE;
							break;
						}
					}
				}

				for (int i = 0; i < boss.bulletList.size(); i++)
				{
					if (SDL_HasIntersection(&player1.posRect, &boss.bulletList[i].posRect)) {
						boss.bulletList[i].Reset();

						player1.playerLives -= 1;

						Mix_PlayChannel(-1, explosionSound, 0);

						MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

						if (player1.playerLives <= 0)
						{
							level2 = false;
							gameState = LOSE;
							break;
						}
					}
				}

				if (SDL_HasIntersection(&player1.posRect, &gauge1.pickupRect)) {
					havegaugeFill1 = true;
					gauge1.active = false;
					gauge1.pickupRect.x = -5000;
					gauge1.pickupRect.y = -5000;
				}

				if (SDL_HasIntersection(&player1.posRect, &gauge2.pickupRect)) {
					havegaugeFill2 = true;
					gauge2.active = false;
					gauge2.pickupRect.x = -5000;
					gauge2.pickupRect.y = -5000;
				}

				if (SDL_HasIntersection(&player1.posRect, &gauge3.pickupRect)) {
					havegaugeFill3 = true;
					gauge3.active = false;
					gauge3.pickupRect.x = -5000;
					gauge3.pickupRect.y = -5000;
				}

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				player1.Draw(renderer);

				turret1.Draw(renderer);

				turret2.Draw(renderer);

				boss.Draw(renderer);

				if (player1.playerScore >= 100)
				{
					if (havegaugeFill1)
						SDL_RenderCopy(renderer, gaugeFill1, NULL, &gaugeFill1Pos);

					if (gauge1.active)
						gauge1.Draw(renderer);
				}

				if (player1.playerScore >= 200)
				{
					if (havegaugeFill2)
						SDL_RenderCopy(renderer, gaugeFill2, NULL, &gaugeFill2Pos);

					if (gauge2.active)
						gauge2.Draw(renderer);
				}

				if (player1.playerScore >= 300)
				{
					if (havegaugeFill3)
						SDL_RenderCopy(renderer, gaugeFill3, NULL, &gaugeFill3Pos);

					if (gauge3.active)
						gauge3.Draw(renderer);
				}

				if (boss.health <= 0)
				{
					level2 = false;
					gameState = WIN;
					break;
				}

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
									MenuOver = false;
								}

								if(backstoryover)
								{
									instructions = false;
									gameState = BACKSTORY;
									backstoryover = false;
								}

								if(startgameover)
								{
									instructions = false;
									gameState = STARTGAME;
									startgameover = false;
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

				startgameover = SDL_HasIntersection(&activePos, &StartGamebuttonPos);

				backstoryover = SDL_HasIntersection(&activePos, &BackStorybuttonPos);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderCopy(renderer, mouse, NULL, &mousePos);

				SDL_RenderCopy(renderer, InsructionsText, NULL, &InsructionsTextPos);

				if(MenuOver)
				{
					SDL_RenderCopy(renderer, MenubuttonO, NULL, &MenubuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, Menubutton, NULL, &MenubuttonPos);
				}

				if(backstoryover)
				{
					SDL_RenderCopy(renderer, BackStorybuttonO, NULL, &BackStorybuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, BackStorybutton, NULL, &BackStorybuttonPos);
				}

				if(startgameover)
				{
					SDL_RenderCopy(renderer, StartGamebuttonO, NULL, &StartGamebuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, StartGamebutton, NULL, &StartGamebuttonPos);
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
								if(MenuOver)
								{
									backstory = false;
									gameState = MENU;
									MenuOver = false;
								}

								if(instructionsover)
								{
									backstory = false;
									gameState = INSTRUCTIONS;
									instructionsover = false;
								}

								if(startgameover)
								{
									backstory = false;
									gameState = STARTGAME;
									startgameover = false;
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

				startgameover = SDL_HasIntersection(&activePos, &StartGamebuttonPos);

				instructionsover = SDL_HasIntersection(&activePos, &InstructionsButtonPos);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderCopy(renderer, mouse, NULL, &mousePos);

				SDL_RenderCopy(renderer, BackStoryText, NULL, &BackStoryTextPos);

				if(MenuOver)
				{
					SDL_RenderCopy(renderer, MenubuttonO, NULL, &MenubuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, Menubutton, NULL, &MenubuttonPos);
				}

				if(instructionsover)
				{
					SDL_RenderCopy(renderer, InstructionsButtonO, NULL, &InstructionsButtonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, InstructionsButton, NULL, &InstructionsButtonPos);
				}

				if(startgameover)
				{
					SDL_RenderCopy(renderer, StartGamebuttonO, NULL, &StartGamebuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, StartGamebutton, NULL, &StartGamebuttonPos);
				}

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
								if(MenuOver)
								{
									win = false;
									gameState = MENU;
									MenuOver = false;
								}

								if(startgameover)
								{
									win = false;
									gameState = STARTGAME;
									startgameover = false;
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

				startgameover = SDL_HasIntersection(&activePos, &StartGamebuttonPos);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderCopy(renderer, mouse, NULL, &mousePos);

				SDL_RenderCopy(renderer, WinText, NULL, &WinTextPos);

				if(MenuOver)
				{
					SDL_RenderCopy(renderer, MenubuttonO, NULL, &MenubuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, Menubutton, NULL, &MenubuttonPos);
				}

				if(startgameover)
				{
					SDL_RenderCopy(renderer, StartGamebuttonO, NULL, &StartGamebuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, StartGamebutton, NULL, &StartGamebuttonPos);
				}

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
								if(MenuOver)
								{
									lose = false;
									gameState = MENU;
									MenuOver = false;
								}

								if(startgameover)
								{
									lose = false;
									gameState = STARTGAME;
									startgameover = false;
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

				startgameover = SDL_HasIntersection(&activePos, &StartGamebuttonPos);

				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				SDL_RenderCopy(renderer, mainmenu, NULL, &menuPos);

				SDL_RenderCopy(renderer, mouse, NULL, &mousePos);

				SDL_RenderCopy(renderer, LoseText, NULL, &LoseTextPos);

				if(MenuOver)
				{
					SDL_RenderCopy(renderer, MenubuttonO, NULL, &MenubuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, Menubutton, NULL, &MenubuttonPos);
				}

				if(startgameover)
				{
					SDL_RenderCopy(renderer, StartGamebuttonO, NULL, &StartGamebuttonPos);
				}
				else
				{
					SDL_RenderCopy(renderer, StartGamebutton, NULL, &StartGamebuttonPos);
				}

				SDL_RenderPresent(renderer);
			}
			break;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;

}
