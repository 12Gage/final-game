#if defined(_WIN32) || (_WIN64)
	#include "SDL.h"
	#include "SDL_image.h"
#endif

#if defined(__APPLE__)
	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"
#endif

#if defined(__linux__)
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
#endif

#include <string>
#include <iostream>
using namespace std;

class Beam{

public:

	bool active;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir, yDir;

	float speed;

	float pos_X, pos_Y;

	Beam(SDL_Renderer * renderer, string filePath, float x, float y);

	void Update(float deltaTime);

	void Draw(SDL_Renderer * renderer);

	void Reset();

	~Beam();

};
