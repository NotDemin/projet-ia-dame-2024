all: main board.o list.o

list.o: list.c list.h
	gcc -c list.c

board.o: board.c board.h
	gcc -c board.c

list: list.c list.h item.h
	gcc -o list list.c

board: board.c board.h
	gcc -o board board.c

main: main.c board.c list.c board.h list.h item.h
	gcc -o main main.c board.c list.c

