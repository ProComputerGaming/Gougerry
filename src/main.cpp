#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "Sprite.hpp"
#include "Player.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const float HEIGHT_OFFSET_MULTIPLIER = .001;
const int HEIGHT_OFFSET = SCREEN_HEIGHT * HEIGHT_OFFSET_MULTIPLIER;
const int HEIGHT_LIMIT = SCREEN_HEIGHT / 2;
const float TPS = 120.0;
const float FPS = 60.0;
const float MILLIS_PER_TICK = (1000.0 / TPS);
const float MILLIS_PER_FRAME = (1000.0 / FPS);
const std::string TITLE = "Gougerry";

Player *gougerry = NULL;
Sprite *background = NULL;

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

Mix_Music *gMusic = NULL;

bool quit = false;
bool canUpdate = true;
unsigned long tickStartTime;
unsigned long frameStartTime;
unsigned long frameEndTime;
long frameTime;

const Uint8* currentKeyStates;

//Starts up SDL and creates window
bool init();

void update();

//Frees media and shuts down SDL
void close();

int main( int argc, char* args[] )
{

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Event handler
		SDL_Event e;

		Uint32 colorKey = SDL_MapRGB(SDL_GetWindowSurface(gWindow)->format, 0xFF, 0, 0xFF);
		gougerry = new Player("../res/Ghougerry.png", gRenderer, 0, SCREEN_HEIGHT - HEIGHT_OFFSET, colorKey);
		background = new Sprite("../res/background.png", gRenderer);
		background->setHeight(SCREEN_HEIGHT);
		background->setWidth(SCREEN_WIDTH);

		Mix_VolumeMusic(MIX_MAX_VOLUME);
    	gMusic = Mix_LoadMUS( "../res/weird.wav" );
   		if( gMusic == NULL )
    	{
        	printf( "Failed to load beat music! Remember to check file format and bitrate! SDL_mixer Error: %s\n", Mix_GetError() );
    	}
    	Mix_PlayMusic(gMusic, -1);
    	tickStartTime = SDL_GetTicks();
		while( !quit )
		{
			frameStartTime = SDL_GetTicks();

			//Handle events on queue
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
			

			if(SDL_GetTicks() - tickStartTime > MILLIS_PER_TICK){
				update();
				tickStartTime = SDL_GetTicks();
			}
			

			//Clear screen
			SDL_RenderClear( gRenderer );

			//Render texture to screen
			SDL_RenderCopy( gRenderer, background->getTexture(), NULL, background->getRect());
			SDL_RenderCopy(	gRenderer, gougerry->getTexture(), NULL, gougerry->getRect());

			//Update screen
			SDL_RenderPresent( gRenderer );

			frameEndTime = SDL_GetTicks();
			frameTime = frameEndTime - frameStartTime;
			if(frameTime < MILLIS_PER_FRAME){
				if(frameTime > 0){
					SDL_Delay(MILLIS_PER_FRAME - frameTime);
					//printf("Actual FPS: %lf\n", 1000.0/(endTime - startTime));
				}
			}
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				 //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
			}
		}
	}

	return success;
}

void update(){

	if(currentKeyStates[SDL_SCANCODE_ESCAPE]){
		quit = true;
		return;
	}

	if( currentKeyStates[ SDL_SCANCODE_UP ] )
    {
    	gougerry->setYSpeed(-5);
    }
    else if(currentKeyStates[SDL_SCANCODE_DOWN]){
    	gougerry->setYSpeed(5);
    }
    else{
    	gougerry->setYSpeed(0);
    } 

    if(currentKeyStates[ SDL_SCANCODE_LEFT ] && currentKeyStates[ SDL_SCANCODE_RIGHT ]){
    	gougerry->setXSpeed(0);	
    }
    else if( currentKeyStates[ SDL_SCANCODE_LEFT ] ){
        gougerry->setXSpeed(-5);
    }
    else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] ){
        gougerry->setXSpeed(5);
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
}

void close()
{
	delete currentKeyStates;
	delete gougerry;
	delete background;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}