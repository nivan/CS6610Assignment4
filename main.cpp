#include "GL/glew.h"
#include <stdio.h>
#include <iostream>
#include "GL/glut.h" 
#include "GL/glui.h"
#include "texture.c"

#include <math.h> 
#define PI 3.14159265
#define NUM_TRANS_SECTIONS 70
#define NUM_LAT_QUADS 20
using namespace std;

int	main_window; 

GLUquadric* light;
GLUquadric* testCylinder;


// the camera info   
float eye[3]; 
float lookat[3]; 

float phaseAngle = 0.0;

//keyboar translations
float xDirTransl = 0.0;
float zDirTransl = 0.0;

//textures
//GLuint floor_normal_map;
GLubyte y[512][512][4];
GLubyte z[256][256][4];
GLubyte u[512][512][4];
GLubyte v[512][512][4];
GLubyte h[512][512][4];

//Fish Display Lists
GLuint wireframe_list;
GLuint filled_list;

//program and shaders
GLuint vertexShader;
GLuint fragmentShader;
GLuint program;

GLuint toonVertexShader;
GLuint toonFragmentShader;
GLuint toonProgram;
GLuint toonTestProgram;

GLuint floorVertexShader;
GLuint floorFragmentShader;
GLuint floorProgram;

GLuint testVertexShader;
GLuint testFragmentShader;
GLuint testProgram;

//uniforms
GLuint eyePosition;
GLuint fishNormalMap;
GLuint isFishVertex;
GLuint fishTexture;
GLuint polygonMode;
GLuint swim;
GLuint phase;
GLuint floorNormalMap;
GLuint sandTexture;
GLuint swimToon;
GLuint phaseToon;

//wire uniforms
GLuint wireXTrans;
GLuint wireZTrans;

//toon uniforms
GLuint toonCameraPosition;
GLuint toonXTrans;
GLuint toonZTrans;

//test program uniforms
GLuint testFloorTexture;
GLuint eyePositionTest;
GLuint testFishTexture;
GLuint testXTrans;
GLuint testZTrans;
GLuint testPhase;
GLuint testSwim;

//attribute
GLuint tangent;
GLuint binormalFish;
GLuint tangentFish;

// pointers for all of the glui controls 
GLUI *glui; 
GLUI_Rollout		*object_rollout; 
GLUI_RadioGroup		*object_type_radio; 
GLUI_Rotation		*object_rotation; 
GLUI_Translation	*object_xz_trans; 
GLUI_Translation	*object_y_trans; 

GLUI_Rollout		*anim_rollout; 
GLUI_Button			*action_button; 

GLUI_Checkbox *draw_floor_check; 
GLUI_Checkbox *draw_object_check; 

// This  checkbox utilizes the callback 
GLUI_Checkbox *use_depth_buffer; 

int live_draw_left_wall;
int live_draw_right_wall;
int live_draw_front_wall;
int live_draw_back_wall;

// the user id's that we can use to identify which control 
// caused the callback to be called 
#define CB_DEPTH_BUFFER 0 
#define CB_ACTION_BUTTON 1 
#define CB_RESET 2 

// walking action variables 
// 
GLfloat step = 0; 
GLfloat live_anim_speed = 5;

// live variables 
// each of these are associated with a control in the interface. 
// when the control is modified, these variables are automatically updated
int live_draw_floor;
int live_shading_mode;
int live_swim;
int live_use_depth_buffer; 
int live_fish_drawing_mode;
//light parameters
GLfloat light_position_XZ[] = { 0.0, 0.0};
GLfloat light_position_Y = 12;


//texture handler and texture functions
typedef struct texture{
	GLuint     name;
	GLubyte*** data;
	GLuint     width;
	GLuint     height;
	GLuint     components;
} texture;

texture* floor_tex;
texture* sand_tex;
texture* fish_tex;
texture* fish_normal_tex;

texture* texture_new(int width, int height){
	texture* new_texture = (texture*)malloc(sizeof(texture));

	new_texture->data = (GLubyte***)malloc(height*sizeof(GLubyte**));

	for(int i = 0 ; i < height ; i++){
		new_texture->data[i] = (GLubyte**)malloc(width*sizeof(GLubyte*));
	}

	for(int i = 0 ; i < height ; i++){
		for(int j = 0 ; j < width ; j++){
			new_texture->data[i][j] = (GLubyte*)malloc(4*sizeof(GLubyte));
		}
	}

	new_texture->width = width;
	new_texture->height = height;

	return new_texture;
}

texture* load_texture(char* file_name){
	printf("loading texture %s\n",file_name);
	int width;
	int height;
	int components;

	unsigned* image = read_texture(file_name, &(width),
			&(height), &(components));

	texture* texture_handler = texture_new(width, height);

	glGenTextures(1,&(texture_handler->name));

	printf("Name %d width %d height %d\n",texture_handler->name,
			texture_handler->width, texture_handler->height);

	for(unsigned int j = 0 ; j < texture_handler->height ; j++){
		for(unsigned int i = 0 ; i < texture_handler->width ; i++){

			unsigned aux = image[j*width + i];
			GLubyte* aux1 = (GLubyte*)(&aux);
			texture_handler->data[j][i][0] = aux1[0];
			texture_handler->data[j][i][1] = aux1[1];
			texture_handler->data[j][i][2] = aux1[2];
			texture_handler->data[j][i][3] = aux1[3];
		}
	}
	printf("texture %s loaded sucessfuly\n",file_name);
	return texture_handler;
}


void normalize(float v[3]) 
{ 
	float l = v[0]*v[0] + v[1]*v[1] + v[2]*v[2]; 
	l = 1 / (float)sqrt(l); 

	v[0] *= l; 
	v[1] *= l; 
	v[2] *= l; 
} 

void crossproduct(float a[3], float b[3], float res[3]) 
{ 
	res[0] = (a[1] * b[2] - a[2] * b[1]); 
	res[1] = (a[2] * b[0] - a[0] * b[2]); 
	res[2] = (a[0] * b[1] - a[1] * b[0]); 
} 

float length(float v[3]) 
{ 
	return (float)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]); 
} 


void myGlutIdle(void) 
{ 
	// make sure the main window is active 
	if (glutGetWindow() != main_window) 
		glutSetWindow(main_window); 

	if(live_swim) phaseAngle += (0.015*live_anim_speed);
	else          phaseAngle = 0.0;
	//if(phaseAngle >= (2*PI)) phaseAngle = 0;

	// if you have moving objects, you can do that here 

	// just keep redrawing the scene over and over 
	glutPostRedisplay(); 
} 


// mouse handling functions for the main window 
// left mouse translates, middle zooms, right rotates 
// keep track of which button is down and where the last position was 
int cur_button = -1; 
int last_x; 
int last_y; 

// catch mouse up/down events 
void myGlutMouse(int button, int state, int x, int y) 
{ 
	if (state == GLUT_DOWN) 
		cur_button = button; 
	else 
	{ 
		if (button == cur_button) 
			cur_button = -1; 
	} 

	last_x = x; 
	last_y = y; 
} 

// catch mouse move events 
void myGlutMotion(int x, int y) 
{ 
	// the change in mouse position 
	int dx = x-last_x; 
	int dy = y-last_y; 

	float scale, len, theta; 
	float neye[3], neye2[3]; 
	float f[3], r[3], u[3]; 

	switch(cur_button) 
	{ 
	case GLUT_LEFT_BUTTON: 
		// translate 
		f[0] = lookat[0] - eye[0]; 
		f[1] = lookat[1] - eye[1]; 
		f[2] = lookat[2] - eye[2]; 
		u[0] = 0; 
		u[1] = 1; 
		u[2] = 0; 

		// scale the change by how far away we are 
		scale = sqrt(length(f)) * 0.007; 

		crossproduct(f, u, r); 
		crossproduct(r, f, u); 
		normalize(r); 
		normalize(u); 

		eye[0] += -r[0]*dx*scale + u[0]*dy*scale; 
		eye[1] += -r[1]*dx*scale + u[1]*dy*scale; 
		eye[2] += -r[2]*dx*scale + u[2]*dy*scale; 

		lookat[0] += -r[0]*dx*scale + u[0]*dy*scale; 
		lookat[1] += -r[1]*dx*scale + u[1]*dy*scale; 
		lookat[2] += -r[2]*dx*scale + u[2]*dy*scale; 

		break; 

	case GLUT_MIDDLE_BUTTON: 
		// zoom 
		f[0] = lookat[0] - eye[0]; 
		f[1] = lookat[1] - eye[1]; 
		f[2] = lookat[2] - eye[2]; 

		len = length(f); 
		normalize(f); 

		// scale the change by how far away we are 
		len -= sqrt(len)*dx*0.03; 

		eye[0] = lookat[0] - len*f[0]; 
		eye[1] = lookat[1] - len*f[1]; 
		eye[2] = lookat[2] - len*f[2]; 

		// make sure the eye and lookat points are sufficiently far away 
		// push the lookat point forward if it is too close 
		if (len < 1) 
		{ 
			printf("lookat move: %f\n", len); 
			lookat[0] = eye[0] + f[0]; 
			lookat[1] = eye[1] + f[1]; 
			lookat[2] = eye[2] + f[2]; 
		} 

		break; 

	case GLUT_RIGHT_BUTTON: 
		// rotate 

		neye[0] = eye[0] - lookat[0]; 
		neye[1] = eye[1] - lookat[1]; 
		neye[2] = eye[2] - lookat[2]; 

		// first rotate in the x/z plane 
		theta = -dx * 0.007; 
		neye2[0] = (float)cos(theta)*neye[0] + (float)sin(theta)*neye[2]; 
		neye2[1] = neye[1]; 
		neye2[2] =-(float)sin(theta)*neye[0] + (float)cos(theta)*neye[2]; 


		// now rotate vertically 
		theta = -dy * 0.007; 

		f[0] = -neye2[0]; 
		f[1] = -neye2[1]; 
		f[2] = -neye2[2]; 
		u[0] = 0; 
		u[1] = 1; 
		u[2] = 0; 
		crossproduct(f, u, r); 
		crossproduct(r, f, u); 
		len = length(f); 
		normalize(f); 
		normalize(u); 

		neye[0] = len * ((float)cos(theta)*f[0] + (float)sin(theta)*u[0]); 
		neye[1] = len * ((float)cos(theta)*f[1] + (float)sin(theta)*u[1]); 
		neye[2] = len * ((float)cos(theta)*f[2] + (float)sin(theta)*u[2]); 

		eye[0] = lookat[0] - neye[0]; 
		eye[1] = lookat[1] - neye[1]; 
		eye[2] = lookat[2] - neye[2]; 

		break; 
	} 


	last_x = x; 
	last_y = y; 

	glutPostRedisplay(); 
} 

// you can put keyboard shortcuts in here 
void myGlutKeyboard(unsigned char key, int x, int y) 
{ 
	switch(key) 
	{ 
	// quit 
	case 27:  
	case 'q': 
	case 'Q': 
		exit(0); 
		break; 

	} 

	glutPostRedisplay(); 
} 

// the window has changed shapes, fix ourselves up 
void myGlutReshape(int	x, int y) 
{ 
	int tx, ty, tw, th; 
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th); 
	glViewport(tx, ty, tw, th); 

	glutPostRedisplay(); 
} 

void configureCamera(){

	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, float(tw)/th, 0.01, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2],  0,1,0);

//	glActiveTexture(GL_TEXTURE1);
//	glMatrixMode(GL_TEXTURE);
//	glLoadIdentity();
//	gluPerspective(90, float(tw)/th, 0.01, 500);
//	glMatrixMode(GL_MODELVIEW);

}

void drawFloor(){
	glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT);
	//glEnable(GL_COLOR_MATERIAL);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floor_tex->name); 
	glUniform1i(floorNormalMap,0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sand_tex->name);
	glUniform1i(sandTexture,1);

	//glEnable(GL_TEXTURE_2D);
	if (live_draw_floor)
	{
		glVertexAttrib3f(tangent,1,0,0);
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.4f, 0.4f, 0.4f);    // note: color is state and only needs to be set once
		glNormal3f(0,1,0);
		glTexCoord2f(0,0);
		glVertex3f(-100, -5, -100);
		glTexCoord2f(20,0);
		glVertex3f( 100, -5, -100);
		glTexCoord2f(20,20);
		glVertex3f( 100, -5,  100);
		glTexCoord2f(0,20);
		glVertex3f(-100, -5,  100);
		glEnd();
	}
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_COLOR_MATERIAL);
}

void drawCylinder(GLint numQuads, GLfloat radius, GLfloat height){

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1,1,1);

	glBegin(GL_QUAD_STRIP);
	for(int i = 0 ; i < numQuads ; i++){
		GLfloat x0 = radius*cos(((GLfloat)2*i*PI)/((GLfloat)numQuads));
		GLfloat y0 = radius*sin(((GLfloat)2*i*PI)/((GLfloat)numQuads));
		GLfloat z0 = height;

		GLfloat x1 = x0;
		GLfloat y1 = y0;
		GLfloat z1 = 0;

		glVertex3f(x0,y0,z0);
		glVertex3f(x1,y1,z1);

	}

	glVertex3f(1,0,height);
	glVertex3f(1,0,0);

	glEnd();
	glPopAttrib();
}

void drawCylinder1(GLint numQuads, GLfloat radius, GLfloat height, GLint stacks){

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1,1,1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for(int j = 0 ; j < stacks ; j++){
		glBegin(GL_QUAD_STRIP);
		for(int i = 0 ; i < numQuads ; i++){
			GLfloat x0 = radius*cos(((GLfloat)2*i*PI)/((GLfloat)numQuads));
			GLfloat y0 = radius*sin(((GLfloat)2*i*PI)/((GLfloat)numQuads));
			GLfloat z0 = (j+1)*(height/((GLfloat) stacks));

			GLfloat x1 = x0;
			GLfloat y1 = y0;
			GLfloat z1 = j*(height/((GLfloat) stacks));

			glVertex3f(x0,y0,z0);
			glVertex3f(x1,y1,z1);

		}

		glVertex3f(1,0,(j+1)*(height/((GLfloat) stacks)));
		glVertex3f(1,0,j*(height/((GLfloat) stacks)));
		glEnd();
	}

	glPopAttrib();
}

void drawFish(int mode){

	glPushAttrib(GL_POLYGON_BIT);


		GLfloat mat_specular[] = {0.297254, 0.30829, 0.306678, 0.8};
		GLfloat mat_diff[] = {0.396, 0.74151, 0.69102, 0.8};
		GLfloat mat_amb[] = {0.1, 0.18725, 0.1745, 0.8};
		GLfloat mat_shininess[] = {12.8};
	//
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//glEnable(GL_COLOR_MATERIAL);
	//glColor4f(0.396, 0.74151, 0.69102, 0.8);

	if(mode == 0 || mode == 2){
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//		//filled frame
		//glCallList(filled_list);
		//drawCylinder1(20,1,10,40);
//			glMatrixMode(GL_MODELVIEW);
//			glPushMatrix();
//			glTranslatef(5,0,0);

		//	glBindTexture(GL_TEXTURE_2D, fish_tex->name);
	//		glEnable(GL_TEXTURE_2D);
	    //glColor4f(1.0,1.0,1.0,1.0);

//			glDisable(GL_TEXTURE_2D);
//			glPopMatrix();
	}
	else if(mode == 1){
		//		//wireframe
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	    glColor3f(1.0,1.0,1.0);
	}
	    gluCylinder(testCylinder, 1,1,10,50,50);
	//	glPopAttrib();
}

void configureLight(){

	GLfloat light_position[4];
	light_position[0] = light_position_XZ[0];
	light_position[2] = -light_position_XZ[1];
	light_position[1] = light_position_Y;
	light_position[3] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	GLfloat white[4];
	white[0] = 1.0;//*light_intensity;
	white[1] = 1.0;//*light_intensity;
	white[2] = 1.0;//*light_intensity;
	white[3] = 1.0;//*light_intensity;
	GLfloat amb[4] = {0.1,0.1,0.1,1.0};

	GLfloat specular[4];
	specular[0] = 1.0;
	specular[1] = 1.0;
	specular[2] = 1.0;
	specular[3] = 1.0;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);


}

void drawLight(){
	glPushAttrib(GL_CURRENT_BIT);

	glColor3f(1.0,1.0,0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(light_position_XZ[0],light_position_Y,-light_position_XZ[1]);
	gluSphere(light, 0.1,30,30);
	glPopMatrix();

	glPopAttrib();
}

// draw the scene 
void myGlutDisplay(	void ) 
{ 
	glClearColor(0.1, 0.1, 0.4, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	configureCamera();
	configureLight();

	glUseProgram(floorProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floor_tex->name);
	drawFloor();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if(live_fish_drawing_mode == 0){
		// phong shading
		// draw some stuff
		//
		//glUseProgram(program);
	    glUseProgram(testProgram);	   

	    //glUniform3f(eyePosition, eye[0],eye[1],eye[2]);	
	    glUniform3f(eyePositionTest, eye[0],eye[1],eye[2]);	
	    glUniform1f(testXTrans, xDirTransl);	
	    glUniform1f(testZTrans, zDirTransl);



	    if(live_swim == 1){
		//	glUniform1i(swim, 1);
		glUniform1i(testSwim, 1);		
	    }
	    else{
		//glUniform1i(swim, 0);
		glUniform1i(testSwim, 0);		
	    }

	    glUniform1f(phase, phaseAngle);
	    glUniform1f(testPhase, phaseAngle);

	    if(live_fish_drawing_mode == 0) glUniform1i(polygonMode, 0);
	    if(live_fish_drawing_mode == 1) glUniform1i(polygonMode, 1);
	    glUniform1i(isFishVertex, 1);

	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, fish_tex->name);
	    glUniform1i(fishTexture,0);

	    glActiveTexture(GL_TEXTURE2);
	    glBindTexture(GL_TEXTURE_2D, fish_normal_tex->name);
	    glUniform1i(fishNormalMap,2);

	    glActiveTexture(GL_TEXTURE4);
	    glBindTexture(GL_TEXTURE_2D, floor_tex->name);
	    glUniform1i(testFloorTexture,2);
	    glUniform1i(testFishTexture,0);


	    drawFish(live_fish_drawing_mode);

	    glUniform1i(isFishVertex, 0);
	}
	if(live_fish_drawing_mode == 1){
	    // wireframe
	    glUseProgram(program);
	    
	    glUniform3f(eyePositionTest, eye[0],eye[1],eye[2]);	
	    glUniform1f(phase, phaseAngle);
	    glUniform1f(wireXTrans, xDirTransl);
	    glUniform1f(wireZTrans, zDirTransl);


	    if(live_swim == 1){
	       	glUniform1i(swim, 1);
	    }
	    else{
		glUniform1i(swim, 0);		
	    }

	    
	    drawFish(live_fish_drawing_mode);	    
	}
	else if(live_fish_drawing_mode == 2){
		//toon shading
	    glUseProgram(toonProgram);	
	    if(live_swim == 1){
		glUniform1i(swimToon, 1);
	    }
	    else{
		glUniform1i(swimToon, 0);
	    }
	    glUniform1f(phaseToon, phaseAngle);
	    glUniform3f(toonCameraPosition, 
		eye[0],eye[1],eye[2]);	
	    glUniform1f(toonXTrans, xDirTransl);	
	    glUniform1f(toonZTrans, zDirTransl);

	    drawFish(live_fish_drawing_mode);
	}
	glPopMatrix();

	glUseProgram(0);
	glDisable(GL_LIGHTING);
	drawLight();

//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glTranslatef(5,0,0);
//
//	glBindTexture(GL_TEXTURE_2D, fish_tex->name);
//	glEnable(GL_TEXTURE_2D);
//
//	gluCylinder(testCylinder, 1,1,10,50,50);
//	glDisable(GL_TEXTURE_2D);
//	glPopMatrix();

	glEnable(GL_LIGHTING);

	glutSwapBuffers();  
} 

// some controls generate a callback when they are changed 
void glui_cb(int control) 
{ 

	switch(control) 
	{ 
	case CB_DEPTH_BUFFER: 
		if (live_use_depth_buffer) 
			glEnable(GL_DEPTH_TEST); 
		else 
			glDisable(GL_DEPTH_TEST); 
		break; 
	case CB_ACTION_BUTTON: 
		if (step < 0) 
		{ 
			step = 0; 
			action_button->set_name("Stop"); 
		} 
		else 
		{ 
			step = -1; 
			action_button->set_name("Start"); 
		} 
		break; 
	case CB_RESET: 
		// 
		// put your reset callback here 
		// 
		break; 
	} 

	glutPostRedisplay(); 
}

GLchar* readFile(char* fileName, GLint *length){
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fileName != NULL) {
		fp = fopen(fileName,"rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}

	*length = count + 1;


	return content;

}

static void show_info_log(
		GLuint object,
		PFNGLGETSHADERIVPROC glGet__iv,
		PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
	GLint log_length;
	char *log;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = (char*)malloc(log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	printf("%s", log);
	free(log);
}

GLuint makeShader(GLenum type, char *fileName){

	GLint length;
	GLchar *source = readFile(fileName, &length);
	GLuint shader;
	GLint shader_ok;

	if (!source)
		return 0;

	//printf("%d\n", length);

	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&source, &length);
	//	free(source);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok) {
		printf("Failed to compile %s:\n", fileName);
		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;

}

GLuint makeProgram(GLuint vertex_shader, GLuint fragment_shader){
	GLint program_ok;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok) {
		printf("Failed to link shader program:\n");
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

void heightToNormal(GLubyte v[512][512][4]){

    int w = 512;//src.get_width();
    int h = 512;//src.get_height();
    // if(scale[0] == 0.f || scale[1] == 0.f || scale[2] == 0.f)
    // {
    //     float a = float(w)/float(h);
    //     if(a < 1.f)
    //     {
    //         scale[0] = 1.f;
    //         scale[1] = 1.f/a;
    //     }
    //     else
    //     {
    //         scale[0] = a;
    //         scale[1] = 1.f;
    //     }
    //     scale[2] = 1.f;
    // }
    // dst.set_size(w,h);
    GLubyte vOutPut[512][512][4];
    
    for(int i=1; i < w-1; i++)
    {
        for(int j = 1; j < h-1; j++)
        {
	    GLfloat dfdi[3];
	    //dfdi[0] = 2.f;
	    //dfdi[1] = 0.f;
	    //dfdi[2] = GLfloat((v[i+1][j][0] - v[i-1][j][0]))/255.f;

	    dfdi[0] = 1.f;
	    dfdi[1] = 0.f;
	    dfdi[2] = GLfloat(3.0*(v[i+1][j][0] - v[i][j][0]))/255.f;

//            vec3f dfdi(2.f, 0.f, float(src(i+1, j  ) - src(i-1, j  ))/255.f);
	    GLfloat dfdj[3];
	    //dfdj[0] = 0.f;
	    //dfdj[1] = 2.f;
	    //dfdj[2] = GLfloat((v[i][j+1][0] - v[i][j-1][0]))/255.f;

	    dfdj[0] = 0.f;
	    dfdj[1] = 1.f;
	    dfdj[2] = GLfloat((v[i][j+1][0] - v[i][j][0]))/255.f;


//            vec3f dfdj(0.f, 2.f, float(src(i  , j+1) - src(i  , j-1))/255.f);
	    GLfloat n[3];
	    crossproduct(dfdi,dfdj,n);
//            vec3f n = dfdi.cross(dfdj);
            //modulate(n, scale);
            normalize(n);
            vOutPut[i][j][0] = GLubyte((n[0]+1)*127.5);//dst(i,j) = range_compress(n);
	    vOutPut[i][j][1] = GLubyte((n[1]+1)*127.5);
	    vOutPut[i][j][2] = GLubyte((n[2]+1)*127.5);
	    vOutPut[i][j][3] = 1.0;
        }
    }
    // microsoft non-ansi c++ scoping concession
    {
        // cheesy boundary cop-out
        for(int i = 0; i < w; i++)
        {
            //dst(i,0)   = dst(i,1);
	    vOutPut[i][0][0] = vOutPut[i][1][0];
	    vOutPut[i][0][1] = vOutPut[i][1][1];
	    vOutPut[i][0][2] = vOutPut[i][1][2];
	    vOutPut[i][0][3] = vOutPut[i][1][3];

            //dst(i,h-1) = dst(i,h-2);
	    vOutPut[i][h-1][0] = vOutPut[i][h-2][0];
	    vOutPut[i][h-1][1] = vOutPut[i][h-2][1];
	    vOutPut[i][h-1][2] = vOutPut[i][h-2][2];
	    vOutPut[i][h-1][3] = vOutPut[i][h-2][3];

        }
        for(int j = 0; j < h; j++)
        {
            //dst(0,j)   = dst(1,j);
	    vOutPut[0][j][0] = vOutPut[1][j][0];
	    vOutPut[0][j][1] = vOutPut[1][j][1];
	    vOutPut[0][j][2] = vOutPut[1][j][2];
	    vOutPut[0][j][3] = vOutPut[1][j][3];

            //dst(w-1,j) = dst(w-2,j);
	    vOutPut[w-1][j][0] = vOutPut[w-2][j][0];
	    vOutPut[w-1][j][1] = vOutPut[w-2][j][1];
	    vOutPut[w-1][j][2] = vOutPut[w-2][j][2];
	    vOutPut[w-1][j][3] = vOutPut[w-2][j][3];
        }
    }
    
    for(int i=0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
	    v[i][j][0] = vOutPut[i][j][0];
	    v[i][j][1] = vOutPut[i][j][1];
	    v[i][j][2] = vOutPut[i][j][2];
	    v[i][j][3] = vOutPut[i][j][3];
	}
    }	
}

void init_textures(){

	//read files
	floor_tex = load_texture("floor_normal_map.rgb");
	sand_tex = load_texture("sand.rgb");
	fish_tex = load_texture("fishColor.rgb");
	fish_normal_tex = load_texture("fish_bump_map.rgb");


	printf("name = %d\n", floor_tex->name);

	//floor texture
	glBindTexture(GL_TEXTURE_2D, floor_tex->name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for(int i = 0 ; i < 512 ; i++){
		for(int j = 0 ; j < 512 ; j++){
			y[i][j][0] = floor_tex->data[i][j][0];
			y[i][j][1] = floor_tex->data[i][j][1];
			y[i][j][2] = floor_tex->data[i][j][2];
			y[i][j][3] = floor_tex->data[i][j][3];
		}
	}

	free(floor_tex->data);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, floor_tex->width,
			floor_tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,y);


	//sand texture
	glBindTexture(GL_TEXTURE_2D, sand_tex->name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for(int i = 0 ; i < 256 ; i++){
		for(int j = 0 ; j < 256 ; j++){
			z[i][j][0] = sand_tex->data[i][j][0];
			z[i][j][1] = sand_tex->data[i][j][1];
			z[i][j][2] = sand_tex->data[i][j][2];
			z[i][j][3] = sand_tex->data[i][j][3];
		}
	}

	free(sand_tex->data);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sand_tex->width,
			sand_tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,z);


	//fish texture
	glBindTexture(GL_TEXTURE_2D, fish_tex->name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for(int i = 0 ; i < 512 ; i++){
		for(int j = 0 ; j < 512 ; j++){
			u[i][j][0] = fish_tex->data[i][j][0];
			u[i][j][1] = fish_tex->data[i][j][1];
			u[i][j][2] = fish_tex->data[i][j][2];
			u[i][j][3] = fish_tex->data[i][j][3];
		}
	}

	free(fish_tex->data);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fish_tex->width,
			fish_tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,u);

	//fish normalMap
		glBindTexture(GL_TEXTURE_2D, fish_normal_tex->name);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		for(int i = 0 ; i < 512 ; i++){
			for(int j = 0 ; j < 512 ; j++){
				v[i][j][0] = fish_normal_tex->data[i][j][0];
				v[i][j][1] = fish_normal_tex->data[i][j][1];
				v[i][j][2] = fish_normal_tex->data[i][j][2];
				v[i][j][3] = fish_normal_tex->data[i][j][3];
			}
		}

		printf("Test before =  %d,%d,%d,%d/n",v[0][0][0],v[0][0][1],v[0][0][2],v[0][0][3]);
		heightToNormal(v);
		printf("Test after =  %d,%d,%d,%d/n",v[0][0][0],v[0][0][1],v[0][0][2],v[0][0][3]);

		free(fish_normal_tex->data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fish_normal_tex->width,
				fish_normal_tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,v);


}

void init( void)
{
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
		//failed to initialize GLEW!
		printf("failed !!!!!!!!!!!!");
	}
	printf("%s",glewGetString(GLEW_VERSION));


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	init_textures();

	testVertexShader = makeShader(GL_VERTEX_SHADER, "testVertexShader.glsl");
	testFragmentShader = makeShader(GL_FRAGMENT_SHADER, "testFragmentShader.glsl");
	testProgram = makeProgram(testVertexShader, testFragmentShader);
	glUseProgram(testProgram);
	testFloorTexture = 
	    glGetUniformLocation(testProgram, "bumpTex");
	testFishTexture = 
	    glGetUniformLocation(testProgram, "fishTex");
	eyePositionTest = 
	    glGetUniformLocation(testProgram, "cameraPosition");	testXTrans = 
	    glGetUniformLocation(testProgram, "xTrans");
	testZTrans = 
	    glGetUniformLocation(testProgram, "zTrans");	
	testPhase = 
	    glGetUniformLocation(testProgram, "phaseAngle");	
	testSwim = 
	    glGetUniformLocation(testProgram, "testSwim");	

	//toon shading program
	toonVertexShader = makeShader(GL_VERTEX_SHADER, "toonVertexShader.glsl");
	toonFragmentShader = makeShader(GL_FRAGMENT_SHADER, "toonFragmentShader.glsl");
	toonProgram = makeProgram(toonVertexShader, toonFragmentShader);
	toonProgram = 
	    makeProgram(toonVertexShader, toonFragmentShader);
	glUseProgram(toonProgram);
	phaseToon = glGetUniformLocation(toonProgram, "phase");
	swimToon = glGetUniformLocation(toonProgram, "swimT");
	toonCameraPosition = 
	    glGetUniformLocation(toonProgram, "cameraPosition");
	toonXTrans = glGetUniformLocation(toonProgram, "xTransToon");
	toonZTrans = glGetUniformLocation(toonProgram, "zTransToon");


	//floor program
	floorVertexShader = makeShader(GL_VERTEX_SHADER, "floorVertexShader.glsl");
	floorFragmentShader = makeShader(GL_FRAGMENT_SHADER, "floorFragmentShader.glsl");
	floorProgram = makeProgram(floorVertexShader, floorFragmentShader);
	glUseProgram(floorProgram);
	floorNormalMap = glGetUniformLocation(floorProgram, "floorNormalMap");
	sandTexture = glGetUniformLocation(floorProgram, "sandTexture");
	tangent = glGetAttribLocation(floorProgram, "Tangent");






	vertexShader = makeShader(GL_VERTEX_SHADER, "vertex_shader.glsl");
	fragmentShader = makeShader(GL_FRAGMENT_SHADER, "fragment_shader.glsl");

	program = makeProgram(vertexShader, fragmentShader);

	glUseProgram(program);

	isFishVertex = glGetUniformLocation(program,"isFishVertex");

	swim = glGetUniformLocation(program,"swim");
	phase = glGetUniformLocation(program,"phase");
	//polygonMode = glGetUniformLocation(program,"polygonMode");
	//fishTexture = glGetUniformLocation(program,"fishTexture");
	//eyePosition = glGetUniformLocation(program,"eyePosition");
	//fishNormalMap = glGetUniformLocation(program,"fishNormalMap");

	wireXTrans = glGetUniformLocation(program,"xTrans");
	wireZTrans = glGetUniformLocation(program,"zTrans");
} 

void processSpecialKeys(int key, int x, int y) {

	switch(key) {
	case GLUT_KEY_UP :
		zDirTransl -= 0.1;
		break;
	case GLUT_KEY_DOWN :
		zDirTransl += 0.1;
		break;
	case GLUT_KEY_LEFT :
		xDirTransl -= 0.1;
		break;
	case GLUT_KEY_RIGHT :
		xDirTransl += 0.1;
		break;
	}
	glutPostRedisplay();
}

// entry point 
int main(int argc,	char* argv[]) 
{ 

	glutInit(&argc, argv);
	// 
	// create the glut window 
	// 
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
	glutInitWindowSize(1000, 600); 
	glutInitWindowPosition(100,100); 
	main_window = glutCreateWindow("Sample Interface"); 
	init();
	//printf("came into main func");
	// 
	// set callbacks 
	// 
	glutDisplayFunc(myGlutDisplay); 
	GLUI_Master.set_glutReshapeFunc(myGlutReshape); 
	GLUI_Master.set_glutIdleFunc(myGlutIdle); 
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard); 
	GLUI_Master.set_glutMouseFunc(myGlutMouse); 
	GLUI_Master.set_glutSpecialFunc(processSpecialKeys);
	glutMotionFunc(myGlutMotion);
	//glutSpecialFunc(processSpecialKeys);


	// 
	// create the interface subwindow and add widgets 
	// 
	glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_LEFT); 


	// initialize live variables 
	live_draw_floor = 1; 
	live_swim = 0;
	live_use_depth_buffer = 1; 
	live_fish_drawing_mode = 0;

	// quit button 
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit); 

	// empty space 
	glui->add_statictext(""); 

	// the object rollout 
	object_rollout = glui->add_rollout("Light");

	// rotation and translation controls 
	// we need an extra panel to keep things grouped properly 
	GLUI_Panel *transform_panel = glui->add_panel_to_panel(object_rollout, "", GLUI_PANEL_NONE);
	object_xz_trans = glui->add_translation_to_panel(transform_panel, "Translate XZ",  
			GLUI_TRANSLATION_XY, light_position_XZ);

	glui->add_column_to_panel(transform_panel, false); 
	object_y_trans =  glui->add_translation_to_panel(transform_panel, "Translate Y",  
			GLUI_TRANSLATION_Y, &light_position_Y);

	object_xz_trans->scale_factor = 0.1f; 
	object_y_trans->scale_factor = 0.1f; 

	glui->add_button_to_panel(object_rollout, "Reset Object Transform", CB_RESET, glui_cb); 

	// empty space 
	glui->add_statictext(""); 



	// the walk control 
	anim_rollout = glui->add_rollout("Swimming Control");
	// our checkbox options for deciding what to draw
	//glui->add_checkbox("Swim", &live_swim);
	glui->add_checkbox_to_panel(anim_rollout,"Swim", &live_swim,12,glui_cb);

	GLUI_Spinner *spin_s = 

			glui->add_spinner_to_panel(anim_rollout, "Speed", GLUI_SPINNER_FLOAT, &live_anim_speed);

	spin_s->set_float_limits(0.1, 10.0); 

	// empty space 
	glui->add_statictext(""); 

	glui->set_main_gfx_window(main_window); 

	// the radio buttons
	object_rollout = glui->add_rollout("Fish");
	object_type_radio = glui->add_radiogroup_to_panel(object_rollout, &live_fish_drawing_mode);
	glui->add_radiobutton_to_group(object_type_radio, "Fill");
	glui->add_radiobutton_to_group(object_type_radio, "Wireframe");
	glui->add_radiobutton_to_group(object_type_radio, "Toon");

	// the radio buttons
	//	object_rollout = glui->add_rollout("Shading");
	//	object_type_radio = glui->add_radiogroup_to_panel(object_rollout, &live_shading_mode);
	//	glui->add_radiobutton_to_group(object_type_radio, "Phong");
	//	glui->add_radiobutton_to_group(object_type_radio, "Toon");


	// initialize the camera 
	eye[0] = 0; 
	eye[1] = 4; 
	eye[2] = 10; //GLuint program;
	lookat[0] = 0; 
	lookat[1] = 0; 
	lookat[2] = 0; 

	light = gluNewQuadric();
	testCylinder = gluNewQuadric();
	gluQuadricOrientation(testCylinder,GLU_OUTSIDE);
	gluQuadricTexture(testCylinder, GL_TRUE);

	// initialize gl 
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_COLOR_MATERIAL); 

	// give control over to glut 
	glutMainLoop(); 

	return 0; 
}
