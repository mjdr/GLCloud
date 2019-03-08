#pragma once
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

typedef struct {
	float x,y,z,rx,ry;
} Camera;

void display_init(int, char *[]);
void display_set_update_func(void (*)());
void display_put_points(const GLfloat*, int);
void display_loop();
Camera* display_camera();
	

