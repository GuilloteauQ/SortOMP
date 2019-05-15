CC=g++
FLAGS=-std=c++11 -fopenmp -fcilkplus

main: main.cpp
	$(CC) main.cpp $(FLAGS) -o main

clean:
	rm main
