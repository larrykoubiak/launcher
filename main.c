#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

int main(int argc, char** argv) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	unsigned int num_fields, column_length,f,i;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,"gamedb.ccyrykcpfupb.eu-central-1.rds.amazonaws.com","d_idea","idea1234","gamedb",0,NULL,0)) {
		printf( "Failed to connect to MySQL: Error: %s\n",mysql_error(conn));
		return 1;
	}
	printf("Logged on to database successfully\nMySQL Server Version is %s\n",mysql_get_server_info(conn));
	if(mysql_query(conn, "select * from tblSystems")) {
		printf( "%s\n",mysql_error(conn));
		return 1;
	}
	res=mysql_use_result(conn);
	num_fields = mysql_num_fields(res);
	printf("Systems:\n");
	for(f=0;f < num_fields;f++){
		field = mysql_fetch_field(res);
		column_length = strlen(field->name);
		if(column_length < field->max_length)
			column_length = field->max_length;
		if(column_length < 4 && field->flags==0)
			column_length = 4;
		field->max_length = column_length;
		printf(" %-*s ",(int)field->max_length,field->name);
	}
	printf("\n");
	while((row=mysql_fetch_row(res)) != NULL) {
		mysql_field_seek(res,0);
		for(i=0;i<num_fields;i++){
			field = mysql_fetch_field(res);
			if( row[i] == NULL)
				printf(" %-*s ",(int)field->max_length,"NULL");
			else if (IS_NUM(field->type))
				printf(" %*s ",(int)field->max_length,row[i]);
			else
				printf(" %-*s ",(int)field->max_length,row[i]);			
		}
		printf("\n");
	}
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}