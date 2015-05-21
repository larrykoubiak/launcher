#include "gamedb.h"

void getSystemFromDb(int systemId,MYSQL *conn, gamedbSystem *sys) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[255];
	sprintf(sql,"SELECT * FROM tblSystems WHERE systemId = %i",systemId);
	mysql_query(conn,sql);
	res=mysql_use_result(conn);
	row=mysql_fetch_row(res);
	sys->id=atoi((char*)row[0]);
	strcpy(sys->name,(char*)row[1]);
	strcpy(sys->manufacturer,(char*)row[2]);
	strcpy(sys->format,(char*)row[3]);
	strcpy(sys->acronym,(char*)row[4]);
	mysql_free_result(res);
}

void getSoftwareFromDb(int softwareId, MYSQL* conn, gamedbSoftware *soft) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[255];
	sprintf(sql,"SELECT * FROM tblSoftwares WHERE softwareId = %i",softwareId);
	mysql_query(conn,sql);
	res=mysql_use_result(conn);
	row=mysql_fetch_row(res);
	soft->id = atoi((char*)row[0]);
	strcpy(soft->name,(char*)row[1]);
	strcpy(soft->type,(char*)row[2]);
	soft->systemId = atoi((char*)row[3]);
	mysql_free_result(res);
}

void getReleaseFromDb(int releaseId, MYSQL* conn, gamedbRelease *rel) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[255];
	sprintf(sql,"SELECT * FROM tblReleases WHERE releaseID = %i",releaseId);
	mysql_query(conn,sql);
	res=mysql_use_result(conn);
	row=mysql_fetch_row(res);
	rel->id = atoi((char*)row[0]);
	strcpy(rel->name,(char*)row[1]);
	strcpy(rel->region,(char*)row[2]);
	rel->softwareId = atoi((char*)row[3]);
	mysql_free_result(res);
}

void getFileFromDb(int fileId, MYSQL* conn, gamedbFile *file) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[255];
	sprintf(sql,"SELECT * FROM tblFiles WHERE fileId = %i",fileId);
	mysql_query(conn,sql);
	res=mysql_use_result(conn);
	row=mysql_fetch_row(res);
	file->id = atoi((char*)row[0]);
	strcpy(file->name,(char*)row[1]);
	file->size = atoi((char*)row[2]);
	strcpy(file->crc,(char*)row[3]);
	strcpy(file->md5,(char*)row[4]);
	strcpy(file->sha1,(char*)row[5]);
	strcpy(file->status,(char*)row[6]);
	file->releaseId = atoi((char*)row[7]);
	mysql_free_result(res);
}