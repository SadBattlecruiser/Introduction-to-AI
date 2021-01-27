CC = g++
FLAGS = -std=gnu++11 -w
HEADERS = -I *.hpp -I src/cimg/ -I src/cimg/plugins/ 
LIBS = -lpng -lz -lm 

all:
	$(CC) $(FLAGS) $(HEADERS) -o main.out src/*.cpp $(LIBS)
