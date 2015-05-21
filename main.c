#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include "gamedb.h"

#define MAX_COL_LENGTH 35

void print_dividing_lines(MYSQL_RES* res) {
	int f,c,num_fields;
	char *col;
	num_fields = mysql_num_fields(res);
	mysql_field_seek(res,0);
	for (f=0;f < num_fields;f++){
		MYSQL_FIELD *field;
		field = mysql_fetch_field(res);
		col = (char*)calloc((int)field->max_length + 1,1);
		col[0] = '+';
		for (c=1;c <(int)field->max_length+1;c++) {
			col[c]='-';
		}
		printf("%s",col);
		free(col);
	}
	printf("+\n");
}

void print_table(char* tablename,MYSQL* conn) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	unsigned int num_fields, column_length,f,i;
	char sql[255];	
	strcpy(sql,"SELECT * FROM ");
	strcat(sql,tablename);
	if(mysql_query(conn, sql)) {
		printf( "%s\n",mysql_error(conn));
	}
	else {
		res=mysql_use_result(conn);
		num_fields = mysql_num_fields(res);
		for(f=0;f < num_fields;f++){
			field = mysql_fetch_field(res);
			column_length = strlen(field->name);
			if(column_length < field->length)
				column_length = field->length;
			if(column_length < 4 && field->flags==0)
				column_length = 4;
			if(column_length > MAX_COL_LENGTH)
				column_length = MAX_COL_LENGTH;
			field->max_length = column_length;
		}
		print_dividing_lines(res);
		mysql_field_seek(res,0);
		for(f=0;f < num_fields;f++) {
			field = mysql_fetch_field(res);
			printf("|%-*s",(int)field->max_length,field->name);
		}
		printf("|\n");
		print_dividing_lines(res);
		while((row=mysql_fetch_row(res)) != NULL) {
			mysql_field_seek(res,0);
			for(i=0;i<num_fields;i++){
				field = mysql_fetch_field(res);
				if( row[i] == NULL)
					printf("|%-*s",(int)field->max_length,"NULL");
				else if (IS_NUM(field->type))
					printf("|%*s",(int)field->max_length,row[i]);
				else
					printf("|%-*.*s",(int)field->max_length,(int)field->max_length,row[i]);			
			}
			printf("|\n");
		}
		print_dividing_lines(res);
		mysql_free_result(res);
	}
}

void prompt_menu(MYSQL* conn) {
	char tblname[50];
	system("clear");
	printf("Enter the table name (exit to quit): ");
	scanf("%50s",tblname);
	while(strcmp(tblname,"exit")!=0) {
		print_table(tblname,conn);
		printf("Enter the table name (exit to quit): ");
		scanf("%50s",tblname);
		system("clear");
	}
}

int main(int argc, char** argv) {
	gamedb *db;
	gamedbSystem *sys;
	gamedbSoftware *soft;
	gamedbRelease *release;
	gamedbFile *file;
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
			}
		}
	}
	puts("\n-- EOF --\n");
	freeGameDb(db);
	/*MYSQL *conn;
	gamedbSystem *sys;
	gamedbSoftware *soft;
	gamedbRelease *release;
	gamedbFile *file;
	gamedbImage *image;
	int r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,"gamedb.ccyrykcpfupb.eu-central-1.rds.amazonaws.com","d_idea","idea1234","gamedb",0,NULL,0)) {
		printf( "Failed to connect to MySQL: Error: %s\n",mysql_error(conn));
		exit(1);
	}
	system("clear");
	puts("\n-- BOF --\n");
	sys=malloc(sizeof(gamedbSystem));
	getSystemFromDb(1,conn,sys);
	printf("\nSystem\n--------\nId: %i\nName: %s\nManufacturer: %s\nFormat: %s\nAcronym: %s\n",sys->id,sys->name,sys->manufacturer,sys->format,sys->acronym);
	soft=malloc(sizeof(gamedbSoftware));
	getSoftwareFromDb(2,conn,soft);
	printf("\nSoftware\n--------\nId: %i\nName: %s\nType: %s\nSystem Id: %i\n",soft->id,soft->name,soft->type,soft->systemId);
	release=malloc(sizeof(gamedbRelease));
	getReleaseFromDb(2,conn,release);
	printf("\nRelease\n--------\nId: %i\nName: %s\nRegion: %s\nSoftware Id: %i\n",release->id,release->name,release->region,release->softwareId);
	file=malloc(sizeof(gamedbFile));
	getFileFromDb(2,conn,file);
	printf("\nFile\n--------\nId: %i\nName: %s\nSize: %i\nCRC: %s\nMD5: %s\nSHA1: %s\nStatus: %s\nRelease Id: %i\n",file->id,file->name,file->size,file->crc,file->md5,file->sha1,file->status,file->releaseId);
	image=malloc(sizeof(gamedbImage));
	getImageFromDb(1,conn,image);
	printf("\nImage\n--------\nId: %i\nName: %s\nSize: %i\nWidth: %i\nHeight: %i\nRelease Id: %i\nSource: %s\n",image->id,image->name,image->size,image->width,image->height,image->releaseTypeId,image->source);
	puts("\n-- EOF --\n");
	free(sys);
	free(soft);
	free(release);
	free(file);
	mysql_close(conn);*/
	return 0;
}