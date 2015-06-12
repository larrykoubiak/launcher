#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gamedb.h"
#include "interface.h"

int main(int argc, char** argv) {
	//database pointers
	gamedb *db;
	gamedbSystem *cur_sys;
	//Init curses
	init_curses();
	//input variables
	db = malloc(sizeof(gamedb));
	cur_sys = malloc(sizeof(gamedbSystem));
	getGameDb(db);
	select_sys_menu(db,cur_sys);
	print_system(cur_sys);
	endwin();
	freeGameDb(db);
	free(cur_sys);
	return 0;
}