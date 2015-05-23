#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include "gamedb.h"

#define MAX_COL_LENGTH 35

int main(int argc, char** argv) {
	gamedb *db;
	gamedbSystem *sys;
	gamedbSoftware *soft;
	gamedbRelease *release;
	gamedbFile *file;
	gamedbImage *image;
	int i,j,k,l;
	
	db = malloc(sizeof(gamedb));
	getGameDb(db);
	system("clear");
	puts("\n-- BOF --\n");
	for(i=0;i<db->nbSystems;i++) {
		sys = &(db->systems[i]);
		printf("Id: %i Name: %s Manufacturer: %s Format: %s Acronym: %s\n",sys->id,sys->name,sys->manufacturer,sys->format,sys->acronym);	
		for(j=0;j<sys->nbSoftwares;j++) {
			soft = &(sys->softwares[j]);
			printf("  Id: %i Name: %s Type: %s System Id: %i\n",soft->id,soft->name,soft->type,soft->systemId);
			for(k=0;k<soft->nbReleases;k++) {
				release = &(soft->releases[k]);
				printf("    Id: %i Name: %s Region: %s Software Id: %i\n",release->id,release->name,release->region,release->softwareId);
				for(l=0;l<release->nbFiles;l++) {
					file = &(release->files[l]);
					printf("      Id: %i Name: %s Size: %i CRC: %s MD5: %s SHA1: %s Status: %s Release Id: %i\n",file->id,file->name,file->size,file->crc,file->md5,file->sha1,file->status,file->releaseId);
				}
				for(l=0;l<release->nbImages;l++) {
					image = &(release->images[l]);
					printf("        Id: %i Name: %s Size: %i Width: %i Height: %i Release Id: %i Source: %s\n",image->id,image->name,image->size,image->width,image->height,image->releaseTypeId,image->source);
				}
			}
		}
	}
	puts("\n-- EOF --\n");
	freeGameDb(db);
	return 0;
}