#ifndef _INCL_GAMEDBINTERFACE
#define _INCL_GAMEDBINTERFACE

#include <menu.h>
#include <curses.h>
#include <string.h>
#include "gamedb.h"

void init_curses();
void select_sys_menu(gamedb *db, gamedbSystem *sys);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void destroy_system_menu(MENU *menu,ITEM **items, int nbitems, WINDOW *win);
void print_system(gamedbSystem *sys);

#endif