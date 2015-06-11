#ifndef _INCL_GAMEDBINTERFACE
#define _INCL_GAMEDBINTERFACE

#include <menu.h>
#include <curses.h>
#include "gamedb.h"

void init_curses();
void show_systems_menu(gamedb *db);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void print_system(gamedbSystem *sys);

#endif