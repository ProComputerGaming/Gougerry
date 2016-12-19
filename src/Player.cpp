#include "Player.hpp"

Player::Player(std::string bmpPath, SDL_Renderer* mainRenderer) : Sprite(bmpPath, mainRenderer){
	health = 100;
	xSpeed = 0;
	ySpeed = 0;
	jumping = false;
}

Player::Player(std::string bmpPath, SDL_Renderer* mainRenderer, Uint32 colorKey) : Sprite(bmpPath, mainRenderer, colorKey){
	health = 100;
	xSpeed = 0;
	ySpeed = 0;
	jumping = false;
}
	
Player::Player(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY) : Sprite(bmpPath, mainRenderer, drawX, drawY){
	health = 100;
	xSpeed = 0;
	ySpeed = 0;
	jumping = false;
}
	
Player::Player(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY, Uint32 colorKey) : Sprite(bmpPath, mainRenderer, drawX, drawY, colorKey){
 	health = 100;
 	xSpeed = 0;
	ySpeed = 0;
	jumping = false;
}

void Player::setXSpeed(int xSpd){
	xSpeed = xSpd;
}

void Player::setYSpeed(int ySpd){
	ySpeed = ySpd;
}

void Player::setHealth(int newHealth){
	health = newHealth;
}

void Player::setJumpStatus(bool jumpStatus){
	jumping = jumpStatus;
}

void Player::setReadyToJump(bool ready){
	readyToJump = ready;
}

int Player::getXSpeed(){
	return xSpeed;
}

int Player::getYSpeed(){
	return ySpeed;
}

int Player::getHealth(){
	return health;
}

bool Player::getJumpStatus(){
	return jumping;
}

bool Player::getReadyToJump(){
	return readyToJump;
}