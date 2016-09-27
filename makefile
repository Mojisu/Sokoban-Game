all	: sokoban.o
	gcc -lncurses -o run sokoban.c

clean:
	rm *.o run -rf
