CC = g++

CFLAGS = -std=c++17

INCLUDES = -I src -I include -I /Library/Frameworks/SDL2.framework/Headers 

SRC_FILES = src/*.cpp

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

TARGET = app

$(TARGET): ./src/*.cpp
	$(CC) $(INCLUDES) $(SRC_FILES) $(CFLAGS) $(LDFLAGS) $(LINKER_FLAGS) -o $@

clean:
	rm *.o app
