#define SDL_MAIN_HANDLED //Needed for windows compilation with mingw32

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
#include "Sprite.hpp"
#include "Player.hpp"

const int SDL_Flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
const int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG;
const int Mix_Flags = MIX_DEFAULT_FORMAT;

const std::string TITLE = "Gougerry";
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float HEIGHT_OFFSET_MULTIPLIER = .001;
const int HEIGHT_OFFSET = SCREEN_HEIGHT * HEIGHT_OFFSET_MULTIPLIER;
const int HEIGHT_LIMIT = SCREEN_HEIGHT / 2;

const float TPS = 120.0;
const float FPS = 60.0;
const float MILLIS_PER_TICK = (1000.0 / TPS);
const float MILLIS_PER_FRAME = (1000.0 / FPS);

const int BULLET_SPEED = 4;
const int BULLET_LIMIT = 5;
const int BULLET_DELAY = 125;

const int X_SPEED = 3;
const int Y_SPEED = 3;

std::vector<SDL_Rect> bullets;

Player *gougerry = nullptr;
Sprite *background = nullptr;
Sprite *bulletSprite = nullptr;
Sprite *splash = nullptr;

SDL_Window *gWindow = nullptr;

SDL_Renderer *gRenderer = nullptr;

SDL_Event e;

Mix_Chunk *intro = nullptr;
Mix_Music *gMusic = nullptr;

bool quit = false;
bool canUpdate = true;

unsigned long lastIntroTime;
unsigned long targetIntroWaitTime;

unsigned long lastBulletTime;
unsigned long tickStartTime;
unsigned long tickEndTime;
unsigned long frameStartTime;
unsigned long frameEndTime;
unsigned long frameTime;

enum GameState{
	INTRO,
	MAIN_MENU,
	LEVEL_ONE
} gameState = INTRO;

enum IntroState{
	ONE,
	TWO,
	THREE,
	FOUR
} introState = ONE;

bool init();

void createBullet();

int update(void* ptr);

void render();

void close();

int main( int argc, char* argv[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{

		Uint32 colorKeyPurple = SDL_MapRGB(SDL_GetWindowSurface(gWindow)->format, 0xFF, 0, 0xFF);
		Uint32 colorKeyWhite = SDL_MapRGB(SDL_GetWindowSurface(gWindow)->format, 0xFF, 0xFF, 0xFF);

		splash = new Sprite("res/Intro.png", gRenderer, colorKeyPurple);

		gougerry = new Player("res/Ghougerry.png", gRenderer, 0, SCREEN_HEIGHT - HEIGHT_OFFSET, colorKeyPurple);
		gougerry->setX(SCREEN_WIDTH/2 - gougerry->getWidth() / 2);

		bulletSprite = new Sprite("res/Bullet.png", gRenderer, colorKeyWhite);
		
		background = new Sprite("res/background.png", gRenderer);
		background->setHeight(SCREEN_HEIGHT);
		background->setWidth(SCREEN_WIDTH);

    	gMusic = Mix_LoadMUS( "res/weird.wav" );
   		if( gMusic == nullptr)
    	{
        	printf( "Failed to load beat music! Remember to check file format and bitrate! SDL_mixer Error: %s\n", Mix_GetError() );
    	}

    	intro = Mix_LoadWAV("res/gougerry.wav");
    	if(intro == nullptr){
    		printf( "Failed to load intro music! Remember to check file format and bitrate! SDL_mixer Error: %s\n", Mix_GetError() );
    	}

    	SDL_Thread* threadID = SDL_CreateThread(update, "UpdateThread", (void*)nullptr);
    	
    	lastIntroTime = SDL_GetTicks();

    	Mix_VolumeChunk(intro, MIX_MAX_VOLUME / 5);
    	Mix_PlayChannel(-1, intro, 0);
		while( !quit )
		{

			frameStartTime = SDL_GetTicks();
			
			render();

			frameEndTime = SDL_GetTicks();
			frameTime = frameEndTime - frameStartTime;
			if(frameTime < MILLIS_PER_FRAME){
				if(frameTime > 0){
					SDL_Delay(MILLIS_PER_FRAME - frameTime);
				}
			}
		}
		SDL_WaitThread(threadID, nullptr);
	}

	close();

	return 0;
}

bool init()
{
	bool success = true;


	if( SDL_Init( SDL_Flags ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}else 
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == nullptr )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == nullptr )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

				
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_mixer
	            if( Mix_OpenAudio( 44100, Mix_Flags, 2, 4096 ) < 0 )
	            {
	               printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	               success = false;
	            }
	            Mix_AllocateChannels(MIX_CHANNELS);
			}
		}
	}
	return success;
}

void createBullet(){
	SDL_Rect bullet;
	bullet.x = gougerry->getX() + (gougerry->getWidth() / 2) - (bulletSprite->getWidth() / 2);
	bullet.y = gougerry->getY() - gougerry->getHeight() / 2;
	bullet.w = bulletSprite->getWidth();
	bullet.h = bulletSprite->getHeight();
	bullets.push_back(bullet);
}

int update(void* ptr){
	const Uint8* currentKeyStates;
	while(!quit){
		switch(gameState){

			case INTRO:
			{
				tickStartTime = SDL_GetTicks();

				while( SDL_PollEvent( &e ) != 0 )
				{
					switch(e.type){
						case SDL_QUIT:
							quit = true;
							break;

						default:
							break;
					}
						
				}

				currentKeyStates = SDL_GetKeyboardState(NULL);

				if(currentKeyStates[SDL_SCANCODE_ESCAPE]){
					quit = true;
					return 0;
				}

				switch(introState){
					case ONE:
						if(SDL_GetTicks() - lastIntroTime > 3500){
							introState = TWO;
							Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
    						Mix_PlayMusic(gMusic, -1);
    						lastIntroTime = SDL_GetTicks();
						}
						break;
					case TWO:
						if(SDL_GetTicks() - lastIntroTime > 4500){
							introState = THREE;
							lastIntroTime = SDL_GetTicks();
						}
						break;

					case THREE:
						if(SDL_GetTicks() - lastIntroTime > 1000){
							introState = FOUR;
							lastIntroTime = SDL_GetTicks();
						}
						break;

					case FOUR:
						if(SDL_GetTicks() - lastIntroTime > 3000){
							gameState = LEVEL_ONE;
							lastIntroTime = SDL_GetTicks();
						}
						break;
					default:
						break;
				}

				unsigned long tickTime = SDL_GetTicks() - tickStartTime;

				if(tickTime < MILLIS_PER_TICK){
					SDL_Delay(MILLIS_PER_TICK - tickTime);
				}
				break;
			}

			case MAIN_MENU:
			{
				tickStartTime = SDL_GetTicks();

				unsigned long tickTime = SDL_GetTicks() - tickStartTime;

				if(tickTime < MILLIS_PER_TICK){
					SDL_Delay(MILLIS_PER_TICK - tickTime);
				}
				break;
			}

			case LEVEL_ONE:
			{
				tickStartTime = SDL_GetTicks();

				while( SDL_PollEvent( &e ) != 0 )
				{
					switch(e.type){
						case SDL_QUIT:
							quit = true;
							break;

						default:
							break;
					}
						
				}


				currentKeyStates = SDL_GetKeyboardState(NULL);

				if(currentKeyStates[SDL_SCANCODE_ESCAPE]){
					quit = true;
					return 0;
				}

				if(currentKeyStates[SDL_SCANCODE_SPACE]){
					if(bullets.size() < BULLET_LIMIT && SDL_GetTicks() - lastBulletTime > BULLET_DELAY){
						lastBulletTime = SDL_GetTicks();
						createBullet();
					}
				}

				if( currentKeyStates[ SDL_SCANCODE_UP ] || currentKeyStates[SDL_SCANCODE_W])
			    {
			    	gougerry->setYSpeed(-Y_SPEED);
			    }
			    else if(currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S]){
			    	gougerry->setYSpeed(Y_SPEED);
			    }
			    else{
			    	gougerry->setYSpeed(0);
			    } 

			    if((currentKeyStates[ SDL_SCANCODE_LEFT ] || currentKeyStates[SDL_SCANCODE_A]) && (currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[SDL_SCANCODE_D])){
			    	gougerry->setXSpeed(0);	
			    }
			    else if( currentKeyStates[ SDL_SCANCODE_LEFT ] || currentKeyStates[SDL_SCANCODE_A]){
			        gougerry->setXSpeed(-X_SPEED);
			    }
			    else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] || currentKeyStates[SDL_SCANCODE_D]){
			        gougerry->setXSpeed(X_SPEED);
			    }
			    else{
			    	gougerry->setXSpeed(0);
			    }

				gougerry->setX(gougerry->getX() + gougerry->getXSpeed());
				gougerry->setY(gougerry->getY() + gougerry->getYSpeed());
				
				if(gougerry->getX() < 0){
					gougerry->setX(0);
				}else if(gougerry->getX() + gougerry->getWidth() > SCREEN_WIDTH){
					gougerry->setX(SCREEN_WIDTH - gougerry->getWidth());
				}

				if(gougerry->getY() < HEIGHT_LIMIT){
					gougerry->setY(HEIGHT_LIMIT);
				}

				if(gougerry->getY() > (SCREEN_HEIGHT - gougerry->getHeight() - HEIGHT_OFFSET)){
					gougerry->setY(SCREEN_HEIGHT - gougerry->getHeight() - HEIGHT_OFFSET);
				}

				for(int i = 0; i < bullets.size(); i++){
					bullets[i].y -= BULLET_SPEED;
				}

				bullets.erase(std::remove_if( bullets.begin(), bullets.end(), [](const auto& thing) { 
						return thing.y < 0; 
					}),
					
					bullets.end()
				);

				unsigned long tickTime = SDL_GetTicks() - tickStartTime;

				if(tickTime < MILLIS_PER_TICK){
					SDL_Delay(MILLIS_PER_TICK - tickTime);
				}
				break;
			}
			default:
				break;
		}
	}
	delete currentKeyStates;
	return 0;
}

void render(){

		switch(gameState){
			case INTRO:
			{
				SDL_Rect firstSplash;
				firstSplash.x = 0;
				firstSplash.y = 0;
				firstSplash.w = splash->getWidth();
				firstSplash.h = splash->getHeight() / 2;

				SDL_Rect screenOne;
				screenOne.x = 0;
				screenOne.y = 0;
				screenOne.w = SCREEN_WIDTH;
				screenOne.h = SCREEN_HEIGHT/2;

					switch(introState){
						case ONE:
				    		SDL_RenderClear(gRenderer);
    						SDL_RenderCopy(gRenderer, gougerry->getTexture(), nullptr, nullptr);
    						SDL_RenderPresent(gRenderer);
    						break;

    					case TWO:
    						SDL_RenderClear(gRenderer);
    						SDL_RenderCopy(gRenderer, gougerry->getTexture(), nullptr, nullptr);
    						SDL_RenderPresent(gRenderer);
    						break;

    					case THREE:
    						SDL_RenderClear(gRenderer);
    						SDL_RenderCopy(gRenderer, splash->getTexture(), &firstSplash, &screenOne);
    						SDL_RenderPresent(gRenderer);
    						break;

    					case FOUR:
    						SDL_RenderClear(gRenderer);
    						SDL_RenderCopy(gRenderer, splash->getTexture(), nullptr, nullptr);
    						SDL_RenderPresent(gRenderer);
    						break;
					}
				break;
			}

			case MAIN_MENU:
			{

				break;
			}

			case LEVEL_ONE:
			{
				SDL_RenderCopy( gRenderer, background->getTexture(), nullptr, background->getRect());

				for(int i = 0; i < bullets.size(); i++){
					SDL_RenderCopy(gRenderer, bulletSprite->getTexture(), nullptr, &bullets[i]);
				}

				SDL_RenderCopy(	gRenderer, gougerry->getTexture(), nullptr, gougerry->getRect());

				SDL_RenderPresent( gRenderer );

				break;
			}

			default:
				break;
		}
}

void close()
{
	delete splash;
	delete gougerry;
	delete bulletSprite;
	delete background;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = nullptr;
	gRenderer = nullptr;

	Mix_FreeChunk(intro);
	Mix_FreeMusic(gMusic);
	intro = nullptr;
	gMusic = nullptr;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}