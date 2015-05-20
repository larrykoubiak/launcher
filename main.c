#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
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
int main(int argc, char** argv) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	char tblname[50];
	char sql[255];
	unsigned int num_fields, column_length,f,i;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,"gamedb.ccyrykcpfupb.eu-central-1.rds.amazonaws.com","d_idea","idea1234","gamedb",0,NULL,0)) {
		printf( "Failed to connect to MySQL: Error: %s\n",mysql_error(conn));
		exit(1);
	}
	system("clear");
	printf("Enter the table name (exit to quit): ");
	scanf("%50s",tblname);
	while(strcmp(tblname,"exit")!=0) {
		strcpy(sql,"SELECT * FROM ");
		strcat(sql,tblname);
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
		printf("Enter the table name (exit to quit): ");
		scanf("%50s",tblname);
		system("clear");
	}
	mysql_close(conn);
	return 0;
}