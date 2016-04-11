#if defined(_WIN32) || (_WIN64)
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_mixer.h"
	#include "SDL_ttf.h"
#endif

#if defined(__APPLE__)
	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"
	#include "SDL2_mixer/SDL_mixer.h"
	#include "SDL2_ttf/SDL_ttf.h"
#endif

#if defined(__linux__)
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
	#include "SDL2/SDL_mixer.h"
	#include "SDL2/SDL_ttf.h"
#endif

#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#include <vector>
#include "bullet.h"
#include "missile.h"

class Player{

public:

	bool active;

	int playerScore, oldScore, playerLives, oldLives;

	TTF_Font *font;

	SDL_Color colorP1 = {255,255,255,255};

	SDL_Surface *scoreSurface, *livesSurface;

	SDL_Texture *scoreTexture, *livesTexture;

	SDL_Rect scorePos, livesPos;

	string tempScore, tempLives;

	Mix_Chunk *laser;

	vector<Bullet>bulletList;

	vector<Missile>missileList;

	string playerPath;

	int playerNum;

	SDL_Surface *surface;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir;
	float yDir;

	float speed;

	float pos_X, pos_Y;

	Player(SDL_Renderer * renderer, int pNum, string filePath, string audioPath, float x, float y);

	void OnControllerAxis(const SDL_ControllerAxisEvent event);

	void OnControllerButton(const SDL_ControllerButtonEvent event);

	void Update(float deltaTime, SDL_Renderer * renderer);

	void Draw(SDL_Renderer * renderer);

	~Player();

	void UpdateScore(SDL_Renderer *renderer);

	void UpdateLives(SDL_Renderer *renderer);

	void Reset();

private:

	void CreateBullet();
	void CreateMissile();

};
