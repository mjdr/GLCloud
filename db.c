#include "db.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

static int currentID = 0;

static MYSQL* conn;

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

bool db_connect() {
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, "192.168.0.101","game","game", "game_data",3306, NULL, 0)){
		printf("Mysql connection error/n");
		return false;
	}
	return true;
}
int db_getData(GLfloat* buffer, int maxLen){
	char sql[1024];
	snprintf(sql, sizeof(sql),
		"SELECT id, pos_x, pos_y, pos_z FROM ds1_paths WHERE id > %d ORDER BY id ASC LIMIT %d;",
		currentID, maxLen
	);
	if(mysql_query(conn, sql)) {
		
		finish_with_error(conn);
		return 0;
	}
	
	MYSQL_RES *result = mysql_store_result(conn);
	if (result == NULL){
      finish_with_error(conn);
		return 0;
	}
	
	MYSQL_ROW row;
	int count = 0;
	while ((row = mysql_fetch_row(result)) != NULL) 
  {
  	currentID = (int)atoi(row[0]);
    buffer[count++] = (GLfloat)atof(row[1]);
    buffer[count++] = (GLfloat)atof(row[2]);
    buffer[count++] = (GLfloat)atof(row[3]); 	
  }
  mysql_free_result(result);
  
  
	return count / 3;
}

