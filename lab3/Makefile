CC=g++
FLAGS= -c -std=c++11
HEADERS= -I .\src\*.hpp

all: main_out
main_out: main_o perceptron_o
	$(CC) .\\bin\\main.o .\\bin\\perceptron.o -o .\\bin\\main.out
main_o:
	$(CC) $(FLAGS) $(HEADERS) .\\src\\main.cpp -o .\\bin\\main.o
perceptron_o:
	$(CC) $(FLAGS) $(HEADERS) .\\src\\perceptron.cpp -o .\\bin\\perceptron.o
