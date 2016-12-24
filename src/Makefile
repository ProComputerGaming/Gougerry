#OBJS specifies which files to compile as part of the project
OBJS = main.cpp Sprite.cpp Player.cpp

#CC specifies which compiler we're using
CXX = g++

WINDOWS_CXX = x86_64-w64-mingw32-g++

SDL_ROOT_DIR = /usr/local/cross-tools/x86_64-w64-mingw32
#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
LINUX_COMPILER_FLAGS = -Wall -g 
WINDOWS_COMPILER_FLAGS = -Wall 

#LINKER_FLAGS specifies the libraries we're linking against
LINUX_LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer
WINDOWS_LINKER_FLAGS = -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -lmingw32 -lSDL2.dll -lSDL2_mixer.dll -lSDL2_image.dll

#OBJ_NAME specifies the name of our exectuable
LINUX_NAME = ../Gougerry_linux
WINDOWS_NAME = ../Gougerry_windows.exe

#This is the target that compiles our executable
linux : $(OBJS)
	$(CXX) $(LINUX_COMPILER_FLAGS) $(LINUX_LINKER_FLAGS) $(OBJS) -o $(LINUX_NAME)

windows : $(OBJS)
	$(WINDOWS_CXX) $(WINDOWS_COMPILER_FLAGS) $(OBJS) $(WINDOWS_LINKER_FLAGS)  -o $(WINDOWS_NAME)
