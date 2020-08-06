CC := gcc
CFLAGS := -std=c11 -g -Wall -Wextra
LDLIBS := -lncurses -lm

bbtetris: main.c record.o tetrimino.o tetris.o title.o components.o 40line.o marathon.o ultra.o ranking.o help.o graphics.c
	$(CC) $(CFLAGS) -o bbtetris $^ $(LDLIBS)

main.c: view/40line.h view/marathon.h view/ultra.h view/ranking.h view/help.h

record.o: record.h
	$(CC) -c $*.c

tetrimino.o: tetris/tetrimino.h
	$(CC) -c tetris/tetrimino.c

tetris.o: tetris/game.h
	$(CC) -c tetris/game.c -o tetris.o

tetrimino.o tetris.o: graphics.h

title.o: view/title.h
	$(CC) -c view/title.c

components.o: view/components.h
	$(CC) -c view/components.c

40line.o: view/40line.h
	$(CC) -c view/40line.c

marathon.o: view/marathon.h
	$(CC) -c view/marathon.c

ultra.o: view/ultra.h
	$(CC) -c view/ultra.c

ranking.o: view/ranking.h
	$(CC) -c view/ranking.c

help.o: view/help.h
	$(CC) -c view/help.c

title.o 40line.o marathon.o ultra.o ranking.o help.o: view/components.h graphics.h
40line.o marathon.o ultra.o ranking.o: record.h

clean:
	$(RM) *.gch
	$(RM) *.o
	$(RM) bbtetris