#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>
#include "Sprite.hpp"

class Enemy : public Sprite{

private:
  int health;
  int gridXLocation, gridYLocation;

public:
  ~Enemy();
  Enemy(std::string bmpPath, SDL_Renderer* mainRenderer);
	Enemy(std::string bmpPath, SDL_Renderer* mainRenderer, Uint32 colorKey);
  Enemy(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY);
	Enemy(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY, Uint32 colorKey);

  int getHealth();
  int getGridXLocation();
  int getGridYLocation();

  void setHealth(int newHealth);
  void setGridXLocation(int newX, int screenWidth);
  void setGridYLocation(int newY, int screenHeight);

  bool hasCollided(SDL_Rect bullet);
};



#endif
