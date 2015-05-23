#include "gamedb.h"

void getImages(gamedbRelease* release, MYSQL* conn) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int i;
	char sql[255];
	sprintf(sql,"SELECT i.* FROM tblImages i JOIN tblReleaseImages ri ON ri.imageId = i.imageId WHERE ri.releaseId=%i",release->id);
	if(mysql_query(conn,sql)) {
		release->nbImages = 0;
		release->images = NULL;
	} 
	else {
		res=mysql_store_result(conn);
		release->nbImages = (int)mysql_num_rows(res);
		if(release->nbImages==0) {
			release->images = NULL;
			mysql_free_result(res);
		} 
		else {
			release->images = malloc(sizeof(gamedbImage) * release->nbImages);
			for(i=0;i<release->nbImages;i++) {
				row = mysql_fetch_row(res);
				release->images[i].id = atoi((char*)row[0]);
				strcpy(release->images[i].name,(char*)row[1]);
				release->images[i].size = atoi((char*)row[2]);
				release->images[i].width = atoi((char*)row[3]);
				release->images[i].height = atoi((char*)row[4]);
				release->images[i].releaseTypeId = atoi((char*)row[5]);
				strcpy(release->images[i].source,(char*)row[6]);
			}					
			mysql_free_result(res);
		}
	}
}

void getFiles(gamedbRelease* release, MYSQL* conn) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int f;
	char sql[255];
	sprintf(sql,"SELECT * FROM tblFiles WHERE releaseId=%i",release->id);
	if(mysql_query(conn,sql)) {
		release->nbFiles = 0;
		release->files = NULL;
	} 
	else {
		res = mysql_store_result(conn);
		release->nbFiles = (int)mysql_num_rows(res);
		release->files = malloc(sizeof(gamedbFile) * release->nbFiles);
		for(f=0;f<release->nbFiles;f++) {
			row = mysql_fetch_row(res);
			release->files[f].id = atoi((char*)row[0]);
			strcpy(release->files[f].name,(char*)row[1]);
			release->files[f].size = atoi((char*)row[2]);
			strcpy(release->files[f].crc,(char*)row[3]);
			strcpy(release->files[f].md5,(char*)row[4]);
			strcpy(release->files[f].sha1,(char*)row[5]);
			strcpy(release->files[f].status,(char*)row[6]);
			release->files[f].releaseId = atoi((char*)row[7]);
		}
		mysql_free_result(res);
	}
}

void getReleases(gamedbSoftware* software, MYSQL* conn) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int r;
	char sql[255];
	sprintf(sql,"SELECT * FROM tblReleases WHERE softwareId=%i",software->id);
	mysql_query(conn,sql);
	res = mysql_store_result(conn);
	software->nbReleases = (int)mysql_num_rows(res);
	software->releases = malloc(sizeof(gamedbRelease) * software->nbReleases);
	for(r=0;r<software->nbReleases;r++) {
		row = mysql_fetch_row(res);
		software->releases[r].id = atoi((char*)row[0]);
		strcpy(software->releases[r].name,(char*)row[1]);
		strcpy(software->releases[r].region,(char*)row[2]);
		software->releases[r].softwareId = atoi((char*)row[3]);
		getFiles(&(software->releases[r]),conn);
		getImages(&(software->releases[r]),conn);
	}
	mysql_free_result(res);	
}

void getSoftwares(gamedbSystem* system, MYSQL* conn) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int s;
	char sql[255];
	sprintf(sql,"SELECT * FROM tblSoftwares WHERE systemId=%i",system->id);
	mysql_query(conn,sql);
	res = mysql_store_result(conn);
	system->nbSoftwares = (int)mysql_num_rows(res);
	system->softwares = malloc(sizeof(gamedbSoftware) * system->nbSoftwares);
	for(s=0;s<system->nbSoftwares;s++) {
		row = mysql_fetch_row(res);
		system->softwares[s].id = atoi((char*)row[0]);
		strcpy(system->softwares[s].name,(char*)row[1]);
		strcpy(system->softwares[s].type,(char*)row[2]);
		system->softwares[s].systemId = atoi((char*)row[3]);
		getReleases(&(system->softwares[s]),conn);
	}
	mysql_free_result(res);
}

void getSystems(gamedb* db,MYSQL* conn) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int s;
	mysql_query(conn,"SELECT * FROM tblSystems");
	res = mysql_store_result(conn);
	db->nbSystems = (int)mysql_num_rows(res);
	db->systems = malloc(sizeof(gamedbSystem) * db->nbSystems);
	for(s=0;s<db->nbSystems;s++){
		row = mysql_fetch_row(res);
		db->systems[s].id=atoi((char*)row[0]);
		strcpy(db->systems[s].name,(char*)row[1]);
		strcpy(db->systems[s].manufacturer,(char*)row[2]);
		strcpy(db->systems[s].format,(char*)row[3]);
		strcpy(db->systems[s].acronym,(char*)row[4]);
		getSoftwares(&(db->systems[s]),conn);
	}
	mysql_free_result(res);
}

void getGameDb(gamedb* db) {
	MYSQL* conn;
	//init mysql
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,"gamedb.ccyrykcpfupb.eu-central-1.rds.amazonaws.com","d_idea","idea1234","gamedb",0,NULL,0)) {
		printf( "Failed to connect to MySQL: Error: %s\n",mysql_error(conn));
		exit(1);
	}
	//get systems
	getSystems(db,conn);
	//cleanup
	mysql_close(conn);
}

void freeGameDb(gamedb* db) {
	gamedbSystem *sys;
	gamedbSoftware *soft;
	gamedbRelease *release;
	int i,j,k;
	for(i=0;i<db->nbSystems;i++) {
		sys = &(db->systems[i]);
		for(j=0;j<sys->nbSoftwares;j++) {
			soft =&(sys->softwares[j]);
			for(k=0;k<soft->nbReleases;k++) {
				release = &(soft->releases[k]);
				if(release->files!=NULL) 
					free(release->files);
				if(release->images!=NULL)
					free(release->images);
			}
			if(soft->releases!=NULL)
				free(soft->releases);
		}
		if(sys->softwares!=NULL)
			free(sys->softwares);
	}
	if(db->systems!=NULL)
		free(db->systems);
	free(db);
}