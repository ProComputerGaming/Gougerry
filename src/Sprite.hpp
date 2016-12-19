#ifndef SPRITE_H
#define SPRITE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

class Sprite{
private:
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;
	Uint32 key;
	std::string filePath;

public:
	~Sprite();
	Sprite(std::string bmpPath, SDL_Renderer* mainRenderer);
	Sprite(std::string bmpPath, SDL_Renderer* mainRenderer, Uint32 colorKey);
	Sprite(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY);
	Sprite(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY, Uint32 colorKey);
	void loadTexture( std::string path, SDL_Renderer* renderer);
	SDL_Surface* getSurface();
	SDL_Texture* getTexture();

	void setWidth(int newWidth);
	void setHeight(int newHeight);
	void setX(int newX);
	void setY(int newY);

	SDL_Rect* getRect();

	int getWidth();
	int getHeight();
	int getX();
	int getY();
};
#endif