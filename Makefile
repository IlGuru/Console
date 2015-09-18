PROJ = console.exe
CC = gcc
OBJS = main.o common.o registry.o screen.o display.o keyboard.o
LIBS = -lcurses -lpthread

console : $(OBJS)
	$(CC) -o $(PROJ) $(OBJS) $(LIBS)

main.o : display.c  main.c  prova_bit_oper.c  registry.c  screen.c  common.c  bit_oper.h  display.h  registry.h  screen.h  common.h
	$(CC) -c main.c
	
common.o : common.c common.h
	$(CC) -c common.c

registry.o : registry.c registry.h common.h
	$(CC) -c registry.c

screen.o : screen.c screen.h display.h common.h
	$(CC) -c screen.c

display.o : display.c display.h bit_oper.h common.h
	$(CC) -c display.c

keyboard.o : keyboard.c keyboard.h bit_oper.h common.h
	$(CC) -c keyboard.c
	
.PHONY: clean
clean:
	-rm -v -f $(PROJ) $(OBJS)