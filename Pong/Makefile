CC = g++

CFLAGS = -std=c++11

#-I /Library/Frameworks/SDL2 is not necessary anymore??
INCLUDES = -I src -I include -I /Library/Frameworks/SDL2.framework/Headers 

#-I /usr/local/Cellar/sdl2/2.28.3/include #-L /usr/local/Cellar/sdl2/2.28.3/lib 
#-I /Library/Frameworks/SDL2.framework/Headers 
#-I /Library/Frameworks/SDL2_image.framework/Headers 

SRC_FILES = src/*.cpp

LDFLAGS = 

#this make dyld sysbol not found error why??
#-framework SDL2 -F /Library/Frameworks
#-framework SDL2_image -F /Library/Frameworks

LINKER_FLAGS = -lSDL2 -lSDL2_image

TARGET = app

$(TARGET): ./src/*.cpp
	$(CC) $(INCLUDES) $(SRC_FILES) $(CFLAGS) $(LDFLAGS) $(LINKER_FLAGS) -o $@

clean:
	rm *.o app
