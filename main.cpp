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

	SDL_Event e;

	enum GameState{MENU, INSTRUCTIONS, BACKSTORY, PLAYERS1, PLAYERS2,WIN,LOSE};

	GameState gameState = MENU;

	SDL_Texture *bkgd = IMG_LoadTexture(renderer, (images_dir + "background.png").c_str());

	SDL_Rect bkgdRect;

	bkgdRect.x = 0;

	bkgdRect.y = 0;

	bkgdRect.w = 1024;

	bkgdRect.h = 768;

	bool menu, instructions, players1, players2, win, lose, quit;

	quit = false;

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				quit = true;

			}

		}

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer,bkgd,NULL, &bkgdRect);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;

}
