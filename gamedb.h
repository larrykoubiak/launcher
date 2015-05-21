#ifndef _INCL_GAMEDBSYSTEM
#define _INCL_GAMEDBSYSTEM

#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct gamedbFile {
	int id;
	char name[256];
	int size;
	char crc[50];
	char md5[50];
	char sha1[50];
	char status[50];
	int releaseId;
} gamedbFile;

typedef struct gamedbRelease {
	int id;
	char name[256];
	char region[50];
	int softwareId;
	gamedbFile *files;
} gamedbRelease;

typedef struct gamedbSoftware {
	int id;
	char name[256];
	char type[50];
	int systemId;
	gamedbRelease *releases;
} gamedbSoftware;

typedef struct gamedbSystem {
	int id;
	char name[256];
	char manufacturer[256];
	char format[50];
	char acronym[6];
	gamedbSoftware *softwares;
} gamedbSystem;

void getSystemFromDb(int systemId,MYSQL* conn, gamedbSystem* sys);
void getSoftwareFromDb(int softwareId,MYSQL* conn, gamedbSoftware* soft);
void getReleaseFromDb(int releaseId, MYSQL* conn, gamedbRelease *rel);
void getFileFromDb(int fileId, MYSQL* conn, gamedbFile *file);

#endif