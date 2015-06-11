#ifndef _INCL_GAMEDBSYSTEM
#define _INCL_GAMEDBSYSTEM

#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct gamedbImage {
	int id;
	char name[256];
	int size;
	int width;
	int height;
	int releaseTypeId;
	char source[50];
} gamedbImage;

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

typedef struct gamedbReleaseFlag {
	int id;
	char name[50];
} gamedbReleaseFlag;

typedef struct gamedbReleaseData {
	int id;
	int releaseId;
	int softwareFlagId;
	char value[2001];
	char source[50];
} gamedbReleaseData;

typedef struct gamedbRelease {
	int id;
	char name[256];
	char region[50];
	int softwareId;
	int nbFiles;
	int nbImages;
	gamedbFile *files;
	gamedbImage *images;
} gamedbRelease;

typedef struct gamedbSoftwareFlag {
	int id;
	char name[50];
} gamedbSoftwareFlag;

typedef struct gamedbSoftwareData {
	int id;
	int softwareId;
	int softwareFlagId;
	char value[2001];
	char source[50];
} gamedbSoftwareData;

typedef struct gamedbSoftware {
	int id;
	char name[256];
	char type[50];
	int systemId;
	gamedbRelease *releases;
	int nbReleases;
} gamedbSoftware;

typedef struct gamedbSystem {
	int id;
	char name[256];
	char manufacturer[256];
	char format[50];
	char acronym[6];
	gamedbSoftware *softwares;
	int nbSoftwares;
} gamedbSystem;

typedef struct gamedb {
	int nbSystems;
	gamedbSystem* systems;
} gamedb;

void getGameDb(gamedb* db);
void getSystems(gamedb* db,MYSQL* conn);
void getSoftwares(gamedbSystem* system, MYSQL* conn);
void getReleases(gamedbSoftware* software, MYSQL* conn);
void getFiles(gamedbRelease* release, MYSQL* conn);
void getImages(gamedbRelease* release, MYSQL* conn);
void freeGameDb(gamedb* db);

#endif