#ifndef _INCL_GAMEDBSYSTEM
#define _INCL_GAMEDBSYSTEM

#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct gamedbSystem {
	int id;
	char name[256];
	char manufacturer[256];
	char format[50];
	char acronym[6];
} gamedbSystem;

void getSystemFromDb(MYSQL* conn, gamedbSystem* sys);
#endif