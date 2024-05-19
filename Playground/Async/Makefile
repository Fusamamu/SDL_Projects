CC = g++

CFLAGS = -std=c++17

INCLUDES = -I include

SRC_FILES = src/*.cpp

TARGET = app

$(TARGET): $(SRC_FILES)
	$(CC) $(INCLUDE) $(SRC_FILES) $(CFLAGS) -o $@

clean:
	rm *.o app
