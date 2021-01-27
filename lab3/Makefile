CC=g++
FLAGS= -c -std=c++11
HEADERS= -I ./*.hpp
SRC= .

all: main_out clear
main_out: main_o perceptron_o
	$(CC) ./main.o ./perceptron.o -o ./main.out
main_o:
	$(CC) $(FLAGS) $(HEADERS) $(SRC)/main.cpp -o ./main.o
perceptron_o:
	$(CC) $(FLAGS) $(HEADERS) $(SRC)/perceptron.cpp -o ./perceptron.o

clear:
	rm ./*.o
