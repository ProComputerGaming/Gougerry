#include "Enemy.hpp"

Enemy::Enemy(std::string bmpPath, SDL_Renderer* mainRenderer) : Sprite(bmpPath, mainRenderer){
  health = 100;
  gridXLocation = 0;
  gridYLocation = 0;
}

Enemy::Enemy(std::string bmpPath, SDL_Renderer* mainRenderer, Uint32 colorKey) : Sprite(bmpPath, mainRenderer, colorKey){
  health = 100;
  gridXLocation = 0;
  gridYLocation = 0;
}

Enemy::Enemy(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY) : Sprite(bmpPath, mainRenderer, drawX, drawY){
  health = 100;
  gridXLocation = 0;
  gridYLocation = 0;
}

Enemy::Enemy(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY, Uint32 colorKey) : Sprite(bmpPath, mainRenderer, drawX, drawY, colorKey){
  health = 100;
  gridXLocation = 0;
  gridYLocation = 0;
}

int Enemy::getHealth(){
  return health;
}

int Enemy::getGridXLocation(){
  return gridXLocation;
}

int Enemy::getGridYLocation(){
  return gridYLocation;
}

void Enemy::setHealth(int newHealth){
  health = newHealth;
}

void Enemy::setGridXLocation(int newX, int screenWidth){
  gridXLocation = newX;
  setX(gridXLocation * 64);
}

void Enemy::setGridYLocation(int newY, int screenHeight){
  gridYLocation = newY;
  setY(gridYLocation * 64);
}

bool Enemy::hasCollided(SDL_Rect bullet){
  return (bullet.x / 64 == gridXLocation && bullet.y / 64 == gridYLocation);
}
