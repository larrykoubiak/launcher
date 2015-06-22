#include "interface.h"

void init_curses() {
	//Init curses
	initscr();
	start_color();
	cbreak();
	noecho();
	//init colors
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	print_in_middle(stdscr,0,0,COLS,"Loading DB, please wait",COLOR_PAIR(2));
	refresh();
}

void show_main_window(gamedb *db) {
	WINDOW *main_win;
	main_win = newwin(LINES,COLS,0,0);
	keypad(main_win, TRUE);
	wbkgd(main_win,COLOR_PAIR(1));
	box(main_win,0,0);
	refresh();
	print_in_middle(main_win, 1, 0, COLS, "Game DB UI", COLOR_PAIR(2));
	mvwprintw(main_win,LINES-2,2,"Escape to exit, m for menu");
	wrefresh(main_win);
	WinMainProc(main_win,db);
}

void WinMainProc(WINDOW* win,gamedb *db) {
	int ch;
	gamedbSystem *cur_sys;
	WINDOW *syswindow;
	cur_sys = malloc(sizeof(gamedbSystem));
	while((ch = wgetch(win)) !=27){ //Escape character
		switch(ch) {
			case 'm':
				show_sys_menu(db,cur_sys);
				syswindow = show_sys_window(cur_sys);
				wrefresh(win);
			default:
				break;
		}
	}
	if(syswindow!=NULL)
		destroy_window(syswindow);
	free(cur_sys);
	destroy_window(win);
}

void show_sys_menu(gamedb *db, gamedbSystem *sys){
	//Variables
	int ch;
	ITEM **my_items;
	MENU *my_menu;
	WINDOW *my_menu_win;
	ITEM *cur_item;
	//counters
	int i;
	//Init menu
	my_items = (ITEM **)calloc(db->nbSystems + 1, sizeof(ITEM *));
	//Add items
	for(i=0;i<db->nbSystems;i++) {		
		my_items[i] = new_item(db->systems[i].acronym,db->systems[i].name);
		set_item_userptr(my_items[i], &(db->systems[i]));
	}
	my_items[db->nbSystems] = (ITEM* )NULL;
	my_menu = new_menu((ITEM**)my_items);
	//Init menu window
	my_menu_win = newwin(db->nbSystems + 4,48,5,(COLS-48)/2);
	keypad(my_menu_win,TRUE);
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win,db->nbSystems,46,2,2));
	set_menu_mark(my_menu, " * ");
	wbkgd(my_menu_win,COLOR_PAIR(3));
	box(my_menu_win,0,0);
	print_in_middle(my_menu_win, 1, 0, 40, "Systems", COLOR_PAIR(1));

	refresh();
	//Post menu
	post_menu(my_menu);
	wrefresh(my_menu_win);
	
	while((ch = wgetch(my_menu_win)) !=10){
		switch(ch) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			default:
				break;
		}
	}
	cur_item = current_item(my_menu);
	memcpy(sys,(gamedbSystem*)item_userptr(cur_item),sizeof(gamedbSystem));
	unpost_menu(my_menu);
	for(i=0;i<db->nbSystems;i++) {
		free_item(my_items[i]);
	}
	free(my_items);
	free_menu(my_menu);
	destroy_window(my_menu_win);
}

WINDOW* show_sys_window(gamedbSystem *sys) {
	WINDOW *sys_win;
	sys_win = newwin(5,110,2,(COLS-110)/2);
	keypad(sys_win,TRUE);
	wbkgd(sys_win,COLOR_PAIR(1));
	refresh();
	wmove(sys_win,1,1);
	wclrtoeol(sys_win);
	wprintw(sys_win,"%2s | %-50s | %-25s | %-10s | %-7s","Id","Name","Manufacturer","Format","Acronym");
	wmove(sys_win,2,1);
	wclrtoeol(sys_win);
	wprintw(sys_win,"---|----------------------------------------------------|---------------------------|------------|----------");
	wmove(sys_win,3,1);
	wclrtoeol(sys_win);
	wprintw(sys_win,"%2d | %-50s | %-25s | %-10s | %-7s",sys->id,sys->name,sys->manufacturer,sys->format,sys->acronym);	
	box(sys_win,0,0);
	wrefresh(sys_win);
	return sys_win;
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
	wrefresh(win);
}

void destroy_window(WINDOW *win) {
	wclear(win);
	wbkgd(win,COLOR_PAIR(1));
	wrefresh(win);
	delwin(win);	
}