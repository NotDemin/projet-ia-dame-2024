all:
	g++ -o main main.cpp game.c board.c list.c  -lsfml-graphics -lsfml-window -lsfml-system