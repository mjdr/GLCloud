#pragma once

#include <GL/gl.h>
#include <stdbool.h>



void mat_identity(GLfloat*);
void mat_mul(const GLfloat*, const GLfloat*, GLfloat*);
void mat_rotX(float, GLfloat*);
void mat_rotY(float, GLfloat*);
void mat_translate(float, float, float, GLfloat*);
bool mat_cmp(const GLfloat*, const GLfloat*, GLfloat);

