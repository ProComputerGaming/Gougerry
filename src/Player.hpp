#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>
#include "Sprite.hpp"

class Player : public Sprite{
private:
	int health;
	int xSpeed, ySpeed;
	bool readyToJump, jumping;

public:

	Player(std::string bmpPath, SDL_Renderer* mainRenderer);
	Player(std::string bmpPath, SDL_Renderer* mainRenderer, Uint32 colorKey);
	Player(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY);
	Player(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY, Uint32 colorKey);

	void setXSpeed(int xSpd);
	void setYSpeed(int ySpd);
	void setHealth(int newHealth);
	void setJumpStatus(bool jumpStatus);
	void setReadyToJump(bool ready);

	int getXSpeed();
	int getYSpeed();
	int getHealth();
	bool getJumpStatus();
	bool getReadyToJump();
};

#endif