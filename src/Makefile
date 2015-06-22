# Makefile

CC=gcc
CFLAGS=-Wall -I. -I../include/mysql
LIBS= -lmenu -lncurses `mysql_config --libs`

all: launcher

launcher: gamedb.o interface.o main.o
	$(CC) $(CFLAGS) -o ../$@ $? $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

gamedb.o: gamedb.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
interface.o: interface.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *o