#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "mat.h"

#define PI 3.141592

typedef struct {
	const char* name;
	bool (*func)();
} Test;

static bool rotX_t(){
	const float a[] = { 0, PI/2, PI, 3*PI/2};

	const float c1 = cos(a[0]);
	const float s1 = sin(a[0]);
	const float c2 = cos(a[1]);
	const float s2 = sin(a[1]);
	const float c3 = cos(a[2]);
	const float s3 = sin(a[2]);
	const float c4 = cos(a[3]);
	const float s4 = sin(a[3]);

	const GLfloat r[64] = {
		 1,0,0,0,  0,c1,s1,0,  0,-s1,c1,0,  0,0,0,1 ,
		 1,0,0,0,  0,c2,s2,0,  0,-s2,c2,0,  0,0,0,1 ,
		 1,0,0,0,  0,c3,s3,0,  0,-s3,c3,0,  0,0,0,1 ,
		 1,0,0,0,  0,c4,s4,0,  0,-s4,c4,0,  0,0,0,1
	};
	GLfloat c[16];
	
	for(int i = 0;i < 4;i++) {
		mat_rotX(a[i],c);
		if(!mat_cmp(c,&r[16*i], 0.001)) return false;
	}
	
	return true;
}
static bool rotY_t(){
	const float a[] = { 0, PI/2, PI, 3*PI/2};

	const float c1 = cos(a[0]);
	const float s1 = sin(a[0]);
	const float c2 = cos(a[1]);
	const float s2 = sin(a[1]);
	const float c3 = cos(a[2]);
	const float s3 = sin(a[2]);
	const float c4 = cos(a[3]);
	const float s4 = sin(a[3]);

	const GLfloat r[64] = {
		 c1,0,-s1,0,  0,1,0,0,  s1,0,c1,0,  0,0,0,1 ,
		 c2,0,-s2,0,  0,1,0,0,  s2,0,c2,0,  0,0,0,1 ,
		 c3,0,-s3,0,  0,1,0,0,  s3,0,c3,0,  0,0,0,1 ,
		 c4,0,-s4,0,  0,1,0,0,  s4,0,c4,0,  0,0,0,1 ,
	};
	GLfloat c[16];
	
	for(int i = 0;i < 4;i++) {
		mat_rotY(a[i],c);
		if(!mat_cmp(c,&r[16*i], 0.001)) return false;
	}
	
	return true;
}

static bool matmul_t(){
	const GLfloat m1[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};
	const GLfloat m2[16] = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
	const GLfloat r[16] = {440,510,580,650,536,622,708,794,632,734,836,938,728,846,964,1082};
	GLfloat c[16];
	
	mat_mul(m1,m2,c);
	
	return mat_cmp(c,r, 0.001);
}





Test tests[] = {
	{ .name="matmul", .func=matmul_t },
	{ .name="rotX", .func=rotX_t },
	{ .name="rotY", .func=rotY_t },
};


int main(){
	for(int i = 0;i < sizeof(tests)/sizeof(Test);i++)
		printf("%s: %s\n", tests[i].name, tests[i].func() ? "Pass" : "Fail");
	return 0;
}

