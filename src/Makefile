#OBJS specifies which files to compile as part of the project
OBJS = main.cpp Sprite.cpp Player.cpp Enemy.cpp

SDL_DIR_LIN = /usr/include/SDL2
SDL_DIR = ../SDL_MINGW/x86_64-w64-mingw32
#CC specifies which compiler we're using
CXX = g++

WINDOWS_CXX = x86_64-w64-mingw32-g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
LINUX_COMPILER_FLAGS = -g -I/usr/include/SDL2 -D_REENTRANT
WINDOWS_COMPILER_FLAGS = -Wall `$(SDL_DIR)/bin/sdl2-config --cflags --libs` -I$(SDL_DIR)/include

#LINKER_FLAGS specifies the libraries we're linking against
LINUX_LINKER_FLAGS = -L/usr/lib/SDL2 -lSDL2 -lSDL2_image -lSDL2_mixer
WINDOWS_LINKER_FLAGS = `$(SDL_DIR)/bin/sdl2-config --libs` -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -lmingw32 -L$(SDL_DIR)/lib -lSDL2main -lSDL2.dll -lSDL2_mixer.dll -lSDL2_image.dll

#OBJ_NAME specifies the name of our exectuable
LINUX_NAME = ../Gougerry_linux
WINDOWS_NAME = ../Gougerry_windows.exe

#This is the target that compiles our executable

all: linux windows

linux : $(OBJS)
	$(CXX) -o $(LINUX_NAME) $(OBJS) $(LINUX_COMPILER_FLAGS) $(LINUX_LINKER_FLAGS)

windows : $(OBJS)
	$(WINDOWS_CXX) $(WINDOWS_COMPILER_FLAGS) $(OBJS) $(WINDOWS_LINKER_FLAGS) -o $(WINDOWS_NAME)
