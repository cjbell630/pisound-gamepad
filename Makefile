CFLAGS = -l drc -lpthread -lx264 -lm -ldl -lswscale
CC = g++

CPP = src/*.cpp
OBJECTS = $(CPP:.cpp=.o)
HEADERS = $(src/*.h)

all: main

clean:
	rm -f core src/*.o a.out main

main: src/main.cpp src/GamepadScreen.cpp $(HEADERS)
	gcc -c -o src/okayu.o src/okayu.c
	$(CC) $(CFLAGS) src/main.cpp src/GamepadScreen.cpp src/okayu.o -o ../executable

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@