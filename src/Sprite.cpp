#include "Sprite.hpp"

Sprite::~Sprite(){
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    surface = nullptr;
    texture = nullptr;
}

Sprite::Sprite(std::string bmpPath, SDL_Renderer* mainRenderer){
    filePath = bmpPath;
    rect.x = 0;
    rect.y = 0;

    loadTexture(filePath, mainRenderer);

    rect.w = surface->w;
    rect.h = surface->h;

    key = 0;
}

Sprite::Sprite(std::string bmpPath, SDL_Renderer* mainRenderer, Uint32 colorKey){
    filePath = bmpPath;
    rect.x = 0;
    rect.y = 0;

    if(colorKey != 0){
        key = colorKey;
    }else{
        key = 0;
    }

    loadTexture(filePath, mainRenderer);
    rect.w = surface->w;
    rect.h = surface->h;
    
}

Sprite::Sprite(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY){
	filePath = bmpPath;
	rect.x = drawX;
	rect.y = drawY;

	loadTexture(filePath, mainRenderer);

    rect.w = surface->w;
    rect.h = surface->h;
	
    key = 0;
}

Sprite::Sprite(std::string bmpPath, SDL_Renderer* mainRenderer, int drawX, int drawY, Uint32 colorKey){
	filePath = bmpPath;
	rect.x = drawX;
    rect.y = drawY;

    if(colorKey != 0){
        key = colorKey;
    }else{
        key = 0;
    }

	loadTexture(filePath, mainRenderer);
	
    rect.w = surface->w;
    rect.h = surface->h;

}

void Sprite::loadTexture( std::string path, SDL_Renderer* renderer)
{
    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    
    if(key > 0){
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);
    }
    if( loadedSurface == nullptr )
    {
        SDL_Log( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == nullptr )
        {
            SDL_Log( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
    }
    surface = loadedSurface;
    texture = newTexture;
}

void Sprite::loadTexture(SDL_Surface* newSurface, SDL_Texture* newTexture){
    surface = newSurface;
    texture = newTexture;
}

SDL_Texture* Sprite::getTexture(){
	return texture;
}

SDL_Surface* Sprite::getSurface(){
    return surface;
}

void Sprite::setWidth(int newWidth){
    rect.w = newWidth;
}

void Sprite::setHeight(int newHeight){
    rect.h = newHeight;
}

void Sprite::setX(int newX){
    rect.x = newX;
}
void Sprite::setY(int newY){
    rect.y = newY;
}

SDL_Rect* Sprite::getRect(){
    return &rect;
}

int Sprite::getWidth(){
    return rect.w;
}
int Sprite::getHeight(){
    return rect.h;
}
int Sprite::getX(){
    return rect.x;
}
int Sprite::getY(){
    return rect.y;
}