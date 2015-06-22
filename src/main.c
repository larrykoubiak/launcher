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
	//init variables
	db = malloc(sizeof(gamedb));
	//fill db
	getGameDb(db);
	//show main window
	show_main_window(db);
	//cleanup
	endwin();
	freeGameDb(db);
	return 0;
}