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

void getImages(gamedbRelease* release, MYSQL* conn);
void getFiles(gamedbRelease* release, MYSQL* conn);
void getReleases(gamedbSoftware* software, MYSQL* conn);
void getSoftwares(gamedbSystem* system, MYSQL* conn);
void getSystems(gamedb* db,MYSQL* conn);
void getGameDb(gamedb* db);
void freeGameDb(gamedb* db);

void getSystemFromDb(int systemId,MYSQL* conn, gamedbSystem* sys);
void getSoftwareFromDb(int softwareId,MYSQL* conn, gamedbSoftware* soft);
void getReleaseFromDb(int releaseId, MYSQL* conn, gamedbRelease *rel);
void getFileFromDb(int fileId, MYSQL* conn, gamedbFile *file);
void getImageFromDb(int imageId, MYSQL* conn, gamedbImage *image);

#endif