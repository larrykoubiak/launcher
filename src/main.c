#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gamedb.h"
#include "interface.h"

int main(int argc, char** argv) {
	//database pointers
	gamedb *db;
	//Init curses
	init_curses();
	//input variables
	db = malloc(sizeof(gamedb));
	getGameDb(db);
	show_systems_menu(db);
	freeGameDb(db);
	endwin();
	return 0;
}