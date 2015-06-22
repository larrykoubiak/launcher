#ifndef _INCL_GAMEDBINTERFACE
#define _INCL_GAMEDBINTERFACE

#include <menu.h>
#include <curses.h>
#include <string.h>
#include "gamedb.h"

void init_curses();
void show_main_window(gamedb *db);
void WinMainProc(WINDOW* win,gamedb *db);
void destroy_window(WINDOW *win);
WINDOW* show_sys_window(gamedbSystem *sys);
void show_sys_menu(gamedb *db, gamedbSystem *sys);
void destroy_system_menu(MENU *menu,ITEM **items, int nbitems, WINDOW *win);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);


#endif