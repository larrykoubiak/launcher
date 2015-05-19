#include <stdio.h>
#include <mysql/mysql.h>

int main(int argc, char** argv) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,"gamedb.ccyrykcpfupb.eu-central-1.rds.amazonaws.com","d_idea","idea1234","gamedb",0,NULL,0)) {
		printf( "Failed to connect to MySQL: Error: %s\n",mysql_error(conn));
		return 1;
	}
	printf("Logged on to database successfully\nMySQL Server Version is %s\n",mysql_get_server_info(conn));
	if(mysql_query(conn, "show tables")) {
		printf( "%s\n",mysql_error(conn));
		return 1;
	}
	res=mysql_use_result(conn);
	printf("MySQL Tables in gamedb database:\n");
	while((row=mysql_fetch_row(res)) != NULL)
		printf("%s \n",row[0]);
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}