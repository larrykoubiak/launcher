#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <menu.h>
#include <curses.h>
#include "gamedb.h"

#define MAX_COL_LENGTH 35
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void print_in_middle(WINDOW *win, int starty, int startx, int width,char *string, chtype color);
void print_system(gamedbSystem *sys);

int main(int argc, char** argv) {
	//database pointers
	gamedb *db;
	gamedbSystem *sys;
	int ch;
	ITEM **my_items;
	MENU *my_menu;
	WINDOW *my_menu_win;
	ITEM *cur_item;
	//counters
	int i;
	//input variables
	db = malloc(sizeof(gamedb));
	getGameDb(db);
	//Init curses
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	wbkgd(stdscr,COLOR_PAIR(1));
	
	//Init menu
	my_items = (ITEM **)calloc(db->nbSystems + 1, sizeof(ITEM *));
	//Add items
	for(i=0;i<db->nbSystems;i++) {		
		sys = &(db->systems[i]);
		my_items[i] = new_item(sys->acronym,sys->name);
		set_item_userptr(my_items[i], sys);
	}
	my_items[db->nbSystems] = (ITEM* )NULL;
	my_menu = new_menu((ITEM**)my_items);
	//Init menu window
	my_menu_win = newwin(db->nbSystems + 4,50,2,(COLS-50)/2);
	keypad(my_menu_win,TRUE);
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win,db->nbSystems,46,2,2));
	set_menu_mark(my_menu, " * ");
	wbkgd(my_menu_win,COLOR_PAIR(3));
	box(my_menu_win,0,0);
	print_in_middle(my_menu_win, 1, 0, 40, "Systems", COLOR_PAIR(1));
	mvprintw(LINES-2,0,"F1 to exit");
	refresh();
	//Post menu
	post_menu(my_menu);
	wrefresh(my_menu_win);
	
	while((ch = wgetch(my_menu_win)) != KEY_F(1)){
		switch(ch) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10: //Enter
				cur_item = current_item(my_menu);
				sys = (gamedbSystem*)item_userptr(cur_item);
				print_system(sys);
				pos_menu_cursor(my_menu);
				break;
			default:
				break;
		}
	}
	for(i=0;i<db->nbSystems;i++) {
		free_item(my_items[i]);
	}
	freeGameDb(db);
	endwin();
	return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
	int length,x,y;
	float temp;
	
	if(win == NULL)
		win = stdscr;
	getyx(win,y,x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;
	length = strlen(string);
	temp = (width - length)/2;
	x = startx + (int)temp;
	wattron(win, color | A_BOLD);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color | A_BOLD);
	refresh();
}

void print_system(gamedbSystem *sys) {
	move(LINES - 4,0);
	clrtoeol();
	printw("%s",sys->name);
	refresh();
}