/*
 * Assignent 4 Part 4 - David Ross (SCPD)
 */

// freeglut includes all of the other OpenGL headers
#include <GL/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

float near_field = 0.25;
float far_field = 20.0;

float object_distance = -10.0f;
float angle = 0.0f;
float angle2= 0.0f;

bool keyStateArray[256];

GLfloat lightPos1[] = {0.0f, 0.0f, 0.0f, 0.0f};

struct vc {
	float x;
	float y;
	float z;
};

struct fc {
	unsigned v1;
	unsigned t1;
	unsigned n1;
	unsigned v2;
	unsigned t2;
	unsigned n2;
	unsigned v3;
	unsigned t3;
	unsigned n3;
};

struct trs {
	vc v1;
	vc v2;
	vc v3;
};

vector<vc> vcord;
vector<fc> face;
vector<trs> tris;

// w - forward : s - backward : d - rotate right : a - rotate left : o - rotate up : l - rotate down
void KeyboardMovement()
{
	float delta_angle = 2.0f;
	float delta_zoom = 0.05f;

	if(keyStateArray['a']){
		angle += delta_angle;
		if(abs(angle) >= 360.f) angle = 0.0f;
	}

	if(keyStateArray['d']){
		angle -= delta_angle;
		if(abs(angle) >= 360.f) angle = 0.0f;
	}

	if(keyStateArray['w']){
		object_distance += delta_zoom;
		if(abs(object_distance) >= far_field) object_distance = -far_field;
		if(abs(object_distance) <= near_field) object_distance = -near_field;
	}

	if(keyStateArray['s']){
		object_distance -= delta_zoom;
		if(abs(object_distance) >= far_field) object_distance = -far_field;
		if(abs(object_distance) <= near_field) object_distance = -near_field;
	}

	if(keyStateArray['o']){
		angle2 += delta_angle;
		if(abs(angle2) >= 360.f) angle2 = 0.0f;
	}

	if(keyStateArray['l']){
		angle2 -= delta_angle;
		if(abs(angle2) >= 360.f) angle2 = 0.0f;
	}
}

void LoadObj(){
  string line;
  ifstream objfile ("venus.obj");
  char id[3];
  if (objfile.is_open())
  {
    while (getline (objfile,line)){
		char *chline = new char [line.length()+1];
		strcpy (chline, line.c_str());
		if(chline[0]=='#'){
			//skips the comment lines
		}else if(chline[0]=='v' && !((chline[1]=='t') || (chline[1]=='n'))){
			vc vct;
			vct.x = vct.y = vct.z = 0.0f;
			sscanf(chline, "%s %f %f %f", id, &vct.x, &vct.y, &vct.z);
			vcord.push_back(vct);
		}else if(chline[0]=='f'){
			fc fct;
			fct.v1 = fct.v2 = fct.v3 = fct.t1 = fct.t2 = fct.t3 = fct.n1 = fct.n2 = fct.n3 = 0;
			sscanf(chline, "%s %u %u %u", id, &fct.v1, &fct.v2, &fct.v3);
			face.push_back(fct);
		}
		delete[] chline;
    }
    objfile.close();
  }else{
	  cout << "Unable to open file";
	  exit(1);
  }
	cout << face.size() << endl;
	for(unsigned i=0; i<face.size(); i++){

		trs trst;
		trst.v1.x = trst.v1.y = trst.v1.z = trst.v2.x = trst.v2.y = trst.v2.z = trst.v3.x = trst.v3.y = trst.v3.z = 0.0f;

		trst.v1.x = vcord[face[i].v1-1].x;
		trst.v1.y = vcord[face[i].v1-1].y;
		trst.v1.z = vcord[face[i].v1-1].z;
		trst.v2.x = vcord[face[i].v2-1].x;
		trst.v2.y = vcord[face[i].v2-1].y;
		trst.v2.z = vcord[face[i].v2-1].z;
		trst.v3.x = vcord[face[i].v3-1].x;
		trst.v3.y = vcord[face[i].v3-1].y;
		trst.v3.z = vcord[face[i].v3-1].z;
		tris.push_back(trst);
	}
}

void Draw()
{
	glLoadIdentity();

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glTranslatef(0.0f, 0.0f, object_distance);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glRotatef(angle2, 1.0f, 0.0f, 0.0f);

	glColor3f(0.5f, 0.5f, 0.5f);
	Sleep(1);
	for(unsigned i=0; i<tris.size(); i++){

// calculate normals
		vc temp_vec;
		vc nrml_vec;
		float len;
		temp_vec.x = (((tris[i].v2.y-tris[i].v1.y)*(tris[i].v3.z-tris[i].v1.z))-((tris[i].v2.z-tris[i].v1.z)*(tris[i].v3.y-tris[i].v1.y)));
		temp_vec.y = (((tris[i].v2.z-tris[i].v1.z)*(tris[i].v3.x-tris[i].v1.x))-((tris[i].v2.x-tris[i].v1.x)*(tris[i].v3.z-tris[i].v1.z)));
		temp_vec.z = (((tris[i].v2.x-tris[i].v1.x)*(tris[i].v3.y-tris[i].v1.y))-((tris[i].v2.y-tris[i].v1.y)*(tris[i].v3.x-tris[i].v1.x)));
		len = sqrtf((temp_vec.x * temp_vec.x) + (temp_vec.y * temp_vec.y) + (temp_vec.z * temp_vec.z));
		nrml_vec.x = temp_vec.x / len;
		nrml_vec.y = temp_vec.y / len;
		nrml_vec.z = temp_vec.z / len;
// draw obj
		glBegin(GL_TRIANGLES);
			glNormal3f(nrml_vec.x, nrml_vec.y, nrml_vec.z);
			glVertex3f(tris[i].v1.x, tris[i].v1.y, tris[i].v1.z);
			glVertex3f(tris[i].v2.x, tris[i].v2.y, tris[i].v2.z);
			glVertex3f(tris[i].v3.x, tris[i].v3.y, tris[i].v3.z);
		glEnd();		
	}
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0);

	LoadObj();
}

void DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Draw();
    glFlush();
}

void ReshapeCallback(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, near_field, far_field);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -10.5, 0.0, 1.0, 0.0);
}

void KeyboardPressedCallback(unsigned char key, int x, int y)
{
	keyStateArray[key] = true;
	KeyboardMovement();
}

void KeyboardUpCallback(unsigned char key, int x, int y)
{
	keyStateArray[key] = false;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS148 Assignment 4 Part 4");

    init();

	glutDisplayFunc(DisplayCallback);
	glutReshapeFunc(ReshapeCallback);
	glutIdleFunc(DisplayCallback);
	glutKeyboardFunc(KeyboardPressedCallback);
	glutKeyboardUpFunc(KeyboardUpCallback);

    glutMainLoop();
    return 0;
}
