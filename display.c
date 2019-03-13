#include "display.h"
#define GL_GLEXT_PROTOTYPES
#include <error.h>
#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "mat.h"

#define MAX_POINTS_COUNT 500000


#define CHECK_GL_STATUS(T,O,S) { \
  GLint status; \
  glGet##T##iv(O, S, &status); \
  if (GL_FALSE == status) { \
    GLint logLength; \
    glGet##T##iv(O, GL_INFO_LOG_LENGTH, &logLength); \
    char *infoLog = malloc(logLength * sizeof(char)); \
    glGet##T##InfoLog(O, logLength, NULL, infoLog); \
    fprintf(stderr, "%d: %d, %s\n", __LINE__, S, infoLog); \
    free(infoLog); \
  } \
}

#define VERTEX_SHADER " \
  #version 130\n \
  uniform mat4 projection;\n \
  uniform mat4 view;\n \
  in vec3 position; \n\
  void main() { \
  	vec4 p = vec4(position, 1.0); \n\
  	gl_Position = projection*view*p; \
  }"

#define FRAGMENT_SHADER " \
  #version 130\n \
  void main() { \
    gl_FragColor = vec4(vec3(0.5), 1.0); \
  }"

#define MIN(a,b) (a) < (b) ? (a) : (b)

typedef struct {
 int x,y;
} Mouse;


static int screenW = 800;
static int screenH = 600;

static Camera camera = {
	.x = 0, .y = 0, .z = 0, .rx = 0, .ry = 0.5+3.141592
};
static Mouse mouse = {
	.x = 0, .y = 0
};
static GLuint shader;
static GLuint positionLoc;
static GLuint projectionLoc;
static GLuint viewLoc;
static GLuint pointsBuffer;
static GLfloat projectionMat[16]= { 0 };
static GLfloat viewMat[16] = { 0 };
static void (*update)();
static int pointsCount = 0;

static void calc_projection();
static void calc_view();
static GLuint create_shader(GLenum, const char*);
static GLuint create_program(GLuint, GLuint);


void gl_init(void)
{
  GLuint vertexShader = create_shader(GL_VERTEX_SHADER, VERTEX_SHADER);
  GLuint fragmentShader = create_shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
  shader = create_program(vertexShader, fragmentShader);

	positionLoc = glGetAttribLocation(shader, "position");
  
  projectionLoc = glGetUniformLocation(shader, "projection");
  viewLoc = glGetUniformLocation(shader, "view");


  GLfloat* data = (GLfloat*)malloc(MAX_POINTS_COUNT * 3*sizeof(GLfloat));
  glGenBuffers(1, &pointsBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
  glBufferData(GL_ARRAY_BUFFER, MAX_POINTS_COUNT * 3*sizeof(GLfloat), data, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  free(data);
  
}

void display(void)
{

  update();
  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


  calc_view();
  
  glUseProgram(shader);
  
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMat);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat);
  
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  
  
  glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
  glEnableVertexAttribArray(positionLoc);
  glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  
  glDrawArrays(GL_POINTS, 0, MIN(pointsCount,MAX_POINTS_COUNT));
  GLenum err = glGetError();
  for (err; err != GL_NO_ERROR; err = glGetError()) {
    fprintf(stderr, "%d: %s\n", err, gluErrorString(err));
  }

  glDisableVertexAttribArray(positionLoc);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glDisable(GL_BLEND);
  glUseProgram(0);

  glutSwapBuffers();
  glutPostRedisplay();
}

void calc_projection()
{
  const float zNear = 0.01;
  const float zFar = 10000;
  const float fov = 80.0/180.0*3.141592;
  
  const float a = (float) screenW / (float) screenH;
  const float zm =  zNear - zFar;
  const float zp = zFar + zNear;
  const float scaleY = 1.0/tan(fov/2);
  const float scaleX = scaleY / a;
  
	for(int i = 0;i < 16;i++)
			projectionMat[i] = 0;
  
	projectionMat[0] = scaleX;
	projectionMat[5] = scaleY;
	projectionMat[10] = zp/zm;
	projectionMat[11] = -1.0;
	projectionMat[14] = 2*zFar*zNear/zm;	
  
}




void calc_view() 
{
	mat_identity(viewMat);
	
	GLfloat tmp1[16], tmp2[16],tmp3[16];
	
	mat_rotX(camera.rx, tmp1);
	mat_mul(viewMat, tmp1, tmp2);
	
			
	mat_rotY(camera.ry, tmp1);
	mat_mul(tmp2, tmp1, tmp3);
	
	mat_translate(-camera.x, -camera.y, -camera.z, tmp1);
	mat_mul(tmp3, tmp1, viewMat);
}


GLuint create_shader(GLenum shaderType, const char* shaderSource)
{
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, (const GLchar **)&shaderSource, NULL);
  glCompileShader(shader);

  CHECK_GL_STATUS(Shader, shader, GL_COMPILE_STATUS);

  return shader;
}

GLuint create_program(GLuint vertexShader, GLuint fragmentShader)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  CHECK_GL_STATUS(Program, program, GL_LINK_STATUS);

  return program;
}



static void mouse_callback(int x, int y) {
	int centerX = screenW / 2;
  int centerY = screenH / 2;

  if(x - centerX != 0.0f || y - centerY != 0.0f) {
        camera.ry += (x - centerX) * 0.001;
				camera.rx += (y - centerY) * 0.001;
	
        glutWarpPointer(centerX, centerY);
  }
	//if(camera.ry > 3.141592/2) camera.ry = 3.141592/2;
	//if(camera.ry < -3.141592/2) camera.ry = -3.141592/2;
	
	mouse.x = x;
	mouse.y = y;
	
}
static void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  screenW = w;
  screenH = h;
  calc_projection();
}


void display_set_update_func(void(*func)()){
	update = func;
}

Camera* display_camera(){
	return &camera;
}

void display_put_points(const GLfloat* buffer, int count){
	const int bytesPerPoint = 3 * sizeof(GLfloat);
	if(pointsCount + count <= MAX_POINTS_COUNT) {
		
  	glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
		glBufferSubData(
			GL_ARRAY_BUFFER, 
			pointsCount * bytesPerPoint, 
			count * bytesPerPoint,
			buffer
		);
		
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
		pointsCount += count;
		return;
	}
	if(pointsCount < MAX_POINTS_COUNT - 1) {
		int free = MAX_POINTS_COUNT - pointsCount;
		display_put_points(buffer, free);
		display_put_points(buffer+free, count-free);
		return;
	}
	
  glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
	for(int i = 0;i < count;i++){
		int index = rand() % MAX_POINTS_COUNT;
		glBufferSubData(
			GL_ARRAY_BUFFER, 
			index * bytesPerPoint, 
			bytesPerPoint,
			buffer + i
		);
	}
  glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
}
void display_loop() {
	glutMainLoop();
}
void display_init(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(screenW, screenH);
  glutInitWindowPosition(1900-screenW, 400);
  glutCreateWindow("Cloud renderer");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutPassiveMotionFunc(mouse_callback);
  gl_init();
  
}
