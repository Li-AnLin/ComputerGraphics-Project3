#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
//#include <cmath>

#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define PARTSNUM 18
#define BODY 0
#define LEFTHAND 1
#define HEAD 5
#define RIGHTHAND 6
#define LEFTFOOT 12
#define RIGHTFOOT 15

void updateModels();

void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button,int state,int x,int y);

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void FrameMenuEvents(int option);
void ShaderMenuEvents(int option);

void idle(int dummy);

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

void Obj2Buffer();
void load2Buffer( char* obj,int);

void updateObj(int);
void resetObj(int);

bool isFrame;

GLuint VAO;
GLuint VBO;
GLuint uVBO;
GLuint nVBO;
GLuint mVBO;
GLuint UBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];

GLuint shadow_fbo;
GLuint shadow_texture;
GLuint frame_fbo;
GLuint frame_texture;
GLuint frame_Dtexture;

GLuint backProgram;
GLuint frameProgram;
GLuint program;
GLuint shadow_lightProgram;

int pNo;
int bID = -1;
int fID = 0;

float angles[PARTSNUM];
float position = 0.0;
float angle = 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
vec3 eyePosition;
vec3 lightPposition;

float size = 1;
GLfloat movex,movey;
float w, h, mx, my;

int vertices_size[PARTSNUM];
int uvs_size[PARTSNUM];
int normals_size[PARTSNUM];
int materialCount[PARTSNUM];

std::vector<std::string> mtls[PARTSNUM];//use material
std::vector<unsigned int> faces[PARTSNUM];//face count
map<string,vec3> KDs;//mtl-name&Kd
map<string,vec3> KSs;//mtl-name&Ks

mat4 Projection ;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

GLuint timeUniform;
GLuint resolutionUniform;
GLuint mouseUniform;

GLuint u_frameMode;
GLuint u_frameTime;
GLuint u_frameResolution;
GLuint u_frameMouse;

#define WALK 1
#define IDLE 0
int mode;
int action;