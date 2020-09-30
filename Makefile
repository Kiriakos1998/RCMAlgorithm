CC = gcc

make: 
	${CC} main.c -o main -lpthread
makeSequential:
	${CC} main.c -o main 
