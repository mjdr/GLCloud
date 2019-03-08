#include "mat.h"
#include <math.h>


#define ABS(x) ((x) < 0 ? -(x) : (x))

void mat_identity(GLfloat* dest)
{
	for(int i = 0;i < 16;i++)
			dest[i] = i/4==i%4 ? 1 : 0;
} 

void mat_rotX(float a, GLfloat* dest) 
{
	mat_identity(dest);
	dest[5] = cos(a);
	dest[6] = sin(a);
	dest[9] = -sin(a);
	dest[10] = cos(a);
}
void mat_rotY(float a, GLfloat* dest) 
{
	mat_identity(dest);
	
	dest[ 0] = cos(a);
	dest[ 2] = -sin(a);
	dest[ 8] = sin(a);
	dest[10] = cos(a);
}

void mat_translate(float x, float y, float z, GLfloat* dest) 
{
	mat_identity(dest);
	
	dest[12] = x;
	dest[13] = y;
	dest[14] = z;
}
void mat_mul(const GLfloat* a, const GLfloat* b, GLfloat* dest) {
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++){
			dest[4*j+i] = 0;
			for(int k = 0;k < 4;k++)
				dest[4*j+i] += a[4*k+i]*b[4*j+k];
		}
}
bool mat_cmp(const GLfloat* a, const GLfloat* b, float maxDelta) {
	for(int i = 0;i < 16;i++)
		if(ABS(a[i] - b[i]) > maxDelta) return false;
	return true;
}
