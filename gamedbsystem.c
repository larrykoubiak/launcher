#include "gamedbsystem.h"

void getSystemFromDb(MYSQL *conn, gamedbSystem *sys) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_query(conn,"SELECT * FROM tblSystems");
	res=mysql_use_result(conn);
	row=mysql_fetch_row(res);
	sys->id = atoi((char*)row[0]);
	strcpy(sys->name,(char*)row[1]);
	strcpy(sys->manufacturer,(char*)row[2]);
	strcpy(sys->format,(char*)row[3]);
	strcpy(sys->acronym,(char*)row[4]);
	mysql_free_result(res);
}