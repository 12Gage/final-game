#include <string>
#include <iostream>

using namespace std;
#include <vector>

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#endif

#include "turretbullet.h"
class Turrent{

public:
	bool active;

	int health;

	vector<TurretBullet> bulletList;

	string turrentPath;

	SDL_Texture *tBase;
	SDL_Texture *tBarrel;

	SDL_Rect baseRect;

	SDL_Rect barrelRect;

	SDL_Rect posRect;

	float x, y, turrentangle;

	SDL_Point center;

	float fireTime = 0.0f;
	float fireRate = 1000.0f;

	Mix_Chunk *fire;

	Turrent(SDL_Renderer * renderer, string filePath, string audioPath, float x, float y);

	void Update(float deltaTIme, SDL_Rect tankRect);

	void Draw(SDL_Renderer *renderer);

	void Reset();

	void CreateBullet(SDL_Rect target);

	void TankMoveX(float tankSpeed, float deltaTime);

	void TankMoveY(float tankSpeed, float deltaTime);

	void RemoveHealthBullet();

	void RemoveHealthMissile();

	void RemoveHealthBeam();

	void ResetTurret1();

	void ResetTurret2();

	float posB_X, posB_Y, posT_X, posT_Y;
};
