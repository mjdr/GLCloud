#include <stdio.h>
#include <stdbool.h>
#include <GL/glut.h>
#include "display.h"
#include "db.h"

#define DB_UPDATE_SKIP 10

GLfloat p[1024 * 3];
bool firstFill = true;

void update() {
	int points = db_getData(p, sizeof(p)/sizeof(GLfloat)/3);
	
	if(points > 0)
		display_put_points(p, points);
	else
		firstFill = false;
	
	display_camera()->ry = 0.5+3.141592;
	display_camera()->x = 400;
	display_camera()->z = -400;
}

int main(int argc, char *argv[])
{  
	db_connect();
	display_init(argc, argv);
	display_set_update_func(update);
	
	display_loop();
  return 0;
}
