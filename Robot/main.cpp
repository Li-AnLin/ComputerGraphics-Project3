#include "main.h"

vec3 camera = vec3(0,0,20);

GLint MatricesIdx;
GLuint ModelID;
GLuint M_KaID;
GLuint M_KdID;
GLuint M_KsID;
GLuint u_view;
GLuint u_projection;
GLuint u_shadow_matrix;
GLuint u_lightPosition;
GLuint displayMode;

int actionTime = 0;
int frameTime = 0;

void motion(int x, int y) {
	mx = x;
	my = y;
}

int main(int argc, char** argv){
	glutInit(&argc, argv);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lin - Robot");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(motion);
	int ActionMenu,ModeMenu,ShaderMenu, FrameMenu;
	ActionMenu = glutCreateMenu(ActionMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("idle",0);
	glutAddMenuEntry("walk",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line",0);
	glutAddMenuEntry("Fill",1);
	glutAddMenuEntry("Solarization", 2);
	glutAddMenuEntry("Negative", 3);
	glutAddMenuEntry("Black & White", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	FrameMenu = glutCreateMenu(FrameMenuEvents);
	glutAddMenuEntry("None", 0);
	glutAddMenuEntry("Offset", 1);
	glutAddMenuEntry("Mosaics", 2);
	glutAddMenuEntry("PartMosaics", 3);
	glutAddMenuEntry("ColorDistortion", 4);
	glutAddMenuEntry("SpatialDistortion", 5);
	glutAddMenuEntry("Displacement", 6);
	glutAddMenuEntry("Patterns", 7);
	glutAddMenuEntry("Fog", 8);
	glutAddMenuEntry("ShadowClone", 9);
	glutAddMenuEntry("Swirling", 10);
	glutAddMenuEntry("CrossStitching", 11);
	glutAddMenuEntry("EdgeDetectors", 12);
	glutAddMenuEntry("RadialBlurFilter", 13);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ShaderMenu = glutCreateMenu(ShaderMenuEvents);
	glutAddMenuEntry("1", 0);
	glutAddMenuEntry("2", 1);
	glutAddMenuEntry("3", 2);
	glutAddMenuEntry("4", 3);
	glutAddMenuEntry("5", 4);
	glutAddMenuEntry("6", 5);
	glutAddMenuEntry("7", 6);
	glutAddMenuEntry("8", 7);
	glutAddMenuEntry("9", 8);
	glutAddMenuEntry("10", 9);
	glutAddMenuEntry("11", 10);
	glutAddMenuEntry("12", 11);
	glutAddMenuEntry("13", 12);
	glutAddMenuEntry("14", 13);
	glutAddMenuEntry("15", 14);
	glutAddMenuEntry("16", 15);
	glutAddMenuEntry("17", 16);
	glutAddMenuEntry("18", 17);
	glutAddMenuEntry("19", 18);
	glutAddMenuEntry("20", 19);
	glutAddMenuEntry("21", 20);
	glutAddMenuEntry("22", 21);
	glutAddMenuEntry("23", 22);
	glutAddMenuEntry("24", 23);
	glutAddMenuEntry("25", 24);
	glutAddMenuEntry("26", 25);
	glutAddMenuEntry("27", 26);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutCreateMenu(menuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddSubMenu("action",ActionMenu);
	glutAddSubMenu("mode",ModeMenu);
	glutAddSubMenu("frame", FrameMenu);
	glutAddSubMenu("background", ShaderMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutMouseFunc(Mouse);
	glutTimerFunc (100, idle, 0); 
	glutMainLoop();
	return 0;
}
void ChangeSize(int width, int height) {
	w = width;
	h = height;
	if (height == 0) height = 1;
	glViewport(0, 0, width, height);
	Projection = perspective(80.0f, (float)width / height, 0.1f, 100.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_fbo);
	glDeleteTextures(1, &frame_texture);
	glGenTextures(1, &frame_texture);
	glBindTexture(GL_TEXTURE_2D, frame_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frame_texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Mouse(int button,int state,int x,int y){
	if(button == 2) isFrame = false;
}
void idle(int dummy){
	isFrame = true;
	int out = 0;
	if(action == WALK){
		updateObj(dummy);
		out = dummy+1;
		if(out > 24) out = 1;
	}
	else if(action == IDLE){
		resetObj(dummy);
		out = 0;
	}
	glutPostRedisplay();
	actionTime += 1;
	frameTime += 1;
	glutTimerFunc (30, idle, out); //150
}
void resetObj(int f){
	for(int i = 0 ; i < PARTSNUM;i++){
		angles[i] = 0.0f;
	}
}
void updateObj(int frame){
	switch(frame){
	case 0:
		//左手
		angles[2] = -25;	//-45
		//右手

		//腿
		angles[13] = 25;	//45
		
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		angles[1] +=5;		//10
		angles[12] -=10;	//15
		position += 0.1;	//0.1
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		angles[1] -=5;		//10
		angles[12] +=10;	//15
		angles[13] -= 10;	//15
		position -= 0.1;	//0.1
		break;
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		angles[1] -=5;		//10
		angles[12] +=10;	//15
		angles[13] = 0;		//0
		position += 0.1;	//0.1
		break;
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		angles[1] +=5;		//10
		angles[12] -=10;	//15
		angles[13] += 10;	//15
		position -= 0.1;	//0.1
		break;
	}
}

void init(){

	//------------------
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 1);//black screen
	//---------------------

	isFrame = false;
	pNo = 0;
	for(int i = 0;i<PARTSNUM;i++)//初始化角度陣列
		angles[i] = 0.0;

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./shader/DSPhong_Material.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "./shader/DSPhong_Material.fp" },//fragment shader
		{ GL_NONE, NULL }};
	program = LoadShaders(shaders);//讀取shader

	glUseProgram(program);//uniform參數數值前必須先use shader
	
	ModelID =  glGetUniformLocation(program,"Model");
	u_view = glGetUniformLocation(program, "View");
	u_projection = glGetUniformLocation(program, "Projection");
	u_shadow_matrix = glGetUniformLocation(program, "shadow_matrix");
	u_lightPosition = glGetUniformLocation(program, "vLightPosition");
	displayMode = glGetUniformLocation(program, "displayMode");
    M_KaID = glGetUniformLocation(program,"Material.Ka");
	M_KdID = glGetUniformLocation(program,"Material.Kd");
	M_KsID = glGetUniformLocation(program,"Material.Ks");

	//background
	shaders[0].filename = "./shader/back.vs";
	shaders[1].filename = "./shader/back.fs";
	backProgram = LoadShaders(shaders);
	glUseProgram(backProgram);
	timeUniform = glGetUniformLocation(backProgram, "time");
	resolutionUniform = glGetUniformLocation(backProgram, "resolution");
	mouseUniform = glGetUniformLocation(backProgram, "mouse");

	//frame
	shaders[0].filename = "./shader/front.vs";
	shaders[1].filename = "./shader/front.fs";
	frameProgram = LoadShaders(shaders);
	glUseProgram(frameProgram);
	u_frameMode = glGetUniformLocation(frameProgram, "mode");
	u_frameTime = glGetUniformLocation(frameProgram, "time");
	u_frameResolution = glGetUniformLocation(frameProgram, "resolution");
	u_frameMouse = glGetUniformLocation(frameProgram, "mouse");
	glGenFramebuffers(1, &frame_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_fbo);
	glGenTextures(1, &frame_texture);
	glBindTexture(GL_TEXTURE_2D, frame_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 800, 600);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frame_texture, 0);
	glGenTextures(1, &frame_Dtexture);
	glBindTexture(GL_TEXTURE_2D, frame_Dtexture);
	glTexStorage2D(GL_TEXTURE_2D, 11, GL_DEPTH_COMPONENT32F, 2048, 1024);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frame_Dtexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Camera matrix
	Projection = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	View       = glm::lookAt(
		glm::vec3(0,10,25) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);

	//load model VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	Obj2Buffer();

	lightPposition = vec3(40.0f, 40.0f, 40.0f);
}

#define DOR(angle) (angle*3.1415/180);

void display(){
	static const GLfloat ones[] = { 1.0f };
	static const GLfloat zero[] = { 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const mat4 scale_bias_matrix = mat4(
		vec4(0.5f, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, 0.5f, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 0.5f, 0.0f),
		vec4(0.5f, 0.5f, 0.5f, 1.0f)
	);
	static const GLenum buffs[] = { GL_COLOR_ATTACHMENT0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	updateModels();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//--------------
	glBindFramebuffer(GL_FRAMEBUFFER, frame_fbo);
	glViewport(0, 0, w, h);
	glDrawBuffers(1, buffs);
	glClearBufferfv(GL_COLOR, 0, zero);
	glClearBufferfv(GL_DEPTH, 0, ones);
	//--------------
	glUseProgram(backProgram);
	glUniform1f(timeUniform, actionTime / 30.);
	glUniform2f(resolutionUniform, w, h);
	glUniform2f(mouseUniform, mx / w, 1 - my / h);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glClear(GL_DEPTH_BUFFER_BIT);
	//------------------
	glBindFramebuffer(GL_FRAMEBUFFER, frame_fbo);
	glBindVertexArray(VAO);
	glUseProgram(program);//uniform參數數值前必須先use shader
	float eyey = DOR(eyeAngley);
	View       = lookAt(
		               vec3(eyedistance*sin(eyey),2,eyedistance*cos(eyey)) , // Camera is at (0,0,20), in World Space
		               vec3(0,0,0), // and looks at the origin
		               vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		                );
	glUniformMatrix4fv(u_view, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, &Projection[0][0]);
	glUniform3fv(u_lightPosition, 1, &lightPposition[0]);
	glUniform1i(displayMode, mID);
	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	//-------------
	if (mID == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (mID == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//-------------
	GLuint offset[3] = {0,0,0};//offset for vertices , uvs , normals
	for(int i = 0;i < PARTSNUM ;i++){
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset
		//(location,vec3,type,固定點,連續點的偏移量,buffer point)
		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);
		//(location,vec2,type,固定點,連續點的偏移量,point)
		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);
		//(location,vec3,type,固定點,連續點的偏移量,point)
		offset[2] +=  normals_size[i]*sizeof(vec3);
		//draw
		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1,1,1);//because .mtl excluding specular , so give it here.
		for(int j = 0;j <mtls[i].size() ;j++){//
			mtlname = mtls[i][j];	
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j+1]*3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
		
	}//end for loop for updating and drawing model

	//--------------
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//--------------
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(frameProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frame_texture);
	glUniform1i(u_frameMode, fID);
	glUniform1f(u_frameTime, frameTime / 30.);
	glUniform2f(u_frameResolution, w, h);
	glUniform2f(u_frameMouse, mx / w + 0.5, my / h + 0.7);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

void Obj2Buffer(){
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;

	loadMTL("Obj/body.mtl", Kds, Kas, Kss, Materials, texture);
	loadMTL("Obj/left_hand.mtl", Kds, Kas, Kss, Materials, texture);
	loadMTL("Obj/head.mtl", Kds, Kas, Kss, Materials, texture);
	loadMTL("Obj/right_hand.mtl", Kds, Kas, Kss, Materials, texture);
	loadMTL("Obj/left_leet.mtl", Kds, Kas, Kss, Materials, texture);
	loadMTL("Obj/right_leet.mtl", Kds, Kas, Kss, Materials, texture);

	for(int i = 0;i<Materials.size();i++){
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}

	
	load2Buffer("Obj/body.obj", BODY);
	load2Buffer("Obj/left_hand.obj", LEFTHAND);
	load2Buffer("Obj/head.obj", HEAD);
	load2Buffer("Obj/right_hand.obj", RIGHTHAND);
	load2Buffer("Obj/left_leet.obj", LEFTFOOT);
	load2Buffer("Obj/right_leet.obj", RIGHTFOOT);
	
	GLuint totalSize[3] = {0,0,0};
	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){
		totalSize[0] += vertices_size[i]*sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&uVBO);
	glGenBuffers(1,&nVBO);
	//bind vbo ,第一次bind也同等於 create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[0],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[1],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[2],NULL,GL_STATIC_DRAW);
	
	
	for(int i = 0;i < PARTSNUM ;i++){
		glBindBuffer(GL_COPY_WRITE_BUFFER,VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[0],vertices_size[i]*sizeof(vec3));
		offset[0] += vertices_size[i]*sizeof(vec3);
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[1],uvs_size[i]*sizeof(vec2));
		offset[1] += uvs_size[i]*sizeof(vec2);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[2],normals_size[i]*sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER,0);


}

void updateModels(){
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	for(int i = 0 ; i < PARTSNUM;i++){
		Models[i] = mat4(1.0f);
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f); 
	}
	float r,pitch,yaw,roll;
	float alpha, beta ,gamma;

	//Body
	beta = angle;
	Rotatation[BODY] = rotate(beta,0,1,0);
	Translation[BODY] = translate(0,1+position,0);
	Models[BODY] = Translation[BODY]*Rotatation[BODY] * rotate(180,0,1,0);
	//左手=======================================================
	//左上手臂
	yaw = DOR(beta);r = 3.7;
	alpha = -angles[LEFTHAND];
	gamma = 0;
	Rotatation[LEFTHAND] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);//向前旋轉*向右旋轉
	Translation[LEFTHAND] = translate(-2,1.5, 0);

	Models[LEFTHAND] = Models[BODY]*Translation[LEFTHAND]*Rotatation[LEFTHAND];
	
	pitch = DOR(alpha);
	//b = DOR(angles[2]);
	roll = DOR(gamma);
	//============================================================
	//頭==========================================================
	Translation[HEAD] = translate(0, 5.5 ,-0.5);
	Models[HEAD] = Models[BODY]*Translation[HEAD]*Rotatation[HEAD];
	//============================================================
	//右手=========================================================
	gamma = 0;alpha = angles[RIGHTHAND] = angles[LEFTHAND];
	Rotatation[RIGHTHAND] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	Translation[RIGHTHAND] = translate(2,1.5, 0);
	Models[RIGHTHAND] = Models[BODY]*Translation[RIGHTHAND]*Rotatation[RIGHTHAND];

	//=============================================================
	//左腳
	alpha = angles[LEFTFOOT];gamma = 10;
	Rotatation[LEFTFOOT] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	Translation[LEFTFOOT] =translate(0.2,-1.5 + position,0);
	Models[LEFTFOOT] = Translation[LEFTFOOT] * Rotatation[LEFTFOOT] * Models[LEFTFOOT] * rotate(180, 0, 1, 0);

	alpha = angles[RIGHTFOOT] = -angles[LEFTFOOT];
	gamma = -10;
	Rotatation[RIGHTFOOT] = rotate(alpha ,1,0,0)*rotate(gamma ,0,0,1);
	Translation[RIGHTFOOT] =translate(-0.2,-1.5 + position,0);
	Models[RIGHTFOOT] = Translation[RIGHTFOOT] * Rotatation[RIGHTFOOT] * Models[RIGHTFOOT] * rotate(180, 0, 1, 0);

}

void load2Buffer(char* obj,int i){
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals,faces[i],mtls[i]);
	if(!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data起始位置,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 scale(float x,float y,float z){
	vec4 c1 = vec4(x,0,0,0);
	vec4 c2 = vec4(0,y,0,0);
	vec4 c3 = vec4(0,0,z,0);
	vec4 c4 = vec4(0,0,0,1);
	mat4 M = mat4(c1,c2,c3,c4);
	return M;
}

mat4 rotate(float angle,float x,float y,float z){
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r)+(1-cos(r))*x*x,(1-cos(r))*y*x+sin(r)*z,(1-cos(r))*z*x-sin(r)*y,0);
	vec4 c2 = vec4((1-cos(r))*y*x-sin(r)*z,cos(r)+(1-cos(r))*y*y,(1-cos(r))*z*y+sin(r)*x,0);
	vec4 c3 = vec4((1-cos(r))*z*x+sin(r)*y,(1-cos(r))*z*y-sin(r)*x,cos(r)+(1-cos(r))*z*z,0);
	vec4 c4 = vec4(0,0,0,1);
	M = mat4(c1,c2,c3,c4);
	return M;
}
void Keyboard(unsigned char key, int x, int y){
	char strT[30];
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./shader/back.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "" },//fragment shader
		{ GL_NONE, NULL }
	};
	switch(key){
	case 'w':
		eyedistance -= 0.2;
		break;
	case 's':
		eyedistance += 0.2;
		break;
	case 'a':
		eyeAngley -=10;
		break;
	case 'd':
		eyeAngley +=10;
		break;
	case 'r':
		angles[1] -= 5; 
		if(angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[2] -= 5;
		if(angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
		break;
	case 'e':
		break;
	case '4':
		eyePosition.x--;
		break;
	case '6':
		eyePosition.x++;
		break;
	case '8':
		eyePosition.y++;
		break;
	case '5':
		eyePosition.y--;
		break;
	case 'z':
		fID--;
		printf("FM:%d\n", fID);
		break;
	case 'x':
		fID++;
		printf("FM:%d\n", fID);
		break;
	case 'c':
		bID--;
		glDeleteProgram(backProgram);
		sprintf(strT, "./shader/background/glsl%03d.txt", bID);
		shaders[1].filename = strT;
		backProgram = LoadShaders(shaders);
		glUseProgram(backProgram);
		timeUniform = glGetUniformLocation(backProgram, "time");
		resolutionUniform = glGetUniformLocation(backProgram, "resolution");
		mouseUniform = glGetUniformLocation(backProgram, "mouse");
		printf("BID : %d\n", bID);
		break;
	case 'v':
		bID++;
		glDeleteProgram(backProgram);
		sprintf(strT, "./shader/background/glsl%03d.txt", bID);
		shaders[1].filename = strT;
		backProgram = LoadShaders(shaders);
		glUseProgram(backProgram);
		timeUniform = glGetUniformLocation(backProgram, "time");
		resolutionUniform = glGetUniformLocation(backProgram, "resolution");
		mouseUniform = glGetUniformLocation(backProgram, "mouse");
		printf("BID : %d\n", bID);
		break;
	case 'b':
		mID--;
		printf("MID : %d\n", mID);
		break;
	case 'n':
		mID++;
		printf("MID : %d\n", mID);
		break;
	}
	glutPostRedisplay();
}
void menuEvents(int option){}
void ActionMenuEvents(int option){
	switch(option){
	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	}
}
void ModeMenuEvents(int option){	
	mID = option;
	printf("MID : %d\n", mID);
}

void FrameMenuEvents(int option) {
	fID = option;
	printf("FID : %d\n", fID);
}

void ShaderMenuEvents(int option){
	bID = option;
	char strT[30];
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "./shader/back.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "" },//fragment shader
		{ GL_NONE, NULL }
	};
	glDeleteProgram(backProgram);
	sprintf(strT, "./shader/background/glsl%03d.txt", bID);
	shaders[1].filename = strT;
	backProgram = LoadShaders(shaders);
	glUseProgram(backProgram);
	timeUniform = glGetUniformLocation(backProgram, "time");
	resolutionUniform = glGetUniformLocation(backProgram, "resolution");
	mouseUniform = glGetUniformLocation(backProgram, "mouse");
	printf("BID : %d\n", bID);
}
