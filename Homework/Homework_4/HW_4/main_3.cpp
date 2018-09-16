/*
 * Assignent 4 Part 3 - David Ross (SCPD)
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
#include "SimpleImage.h"

using namespace std;

float near_field = 0.25;
float far_field = 20.0;

float object_distance = -10.0f;

float angle = 0.0f;
float angle2= 0.0f;

bool keyStateArray[256];

unsigned cube_texture;

struct vc {
	float x;
	float y;
	float z;
};

struct vt {
	float u;
	float v;
};

struct nv {
	float nx;
	float ny;
	float nz;
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
	vt t1;
	vt t2;
	vt t3;
	nv n1;
	nv n2;
	nv n3;
};

vector<vc> vcord;
vector<vt> tcord;
vector<nv> ncord;
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
  ifstream objfile ("tex-cube.obj");
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
		}else if(chline[0]=='v' && chline[1]=='t'){
			vt vtt;
			vtt.u = vtt.v = 0.0f;
			sscanf(chline, "%s %f %f", id, &vtt.u, &vtt.v);
			tcord.push_back(vtt);
		}else if(chline[0]=='v' && chline[1]=='n'){
			nv nvt;
			nvt.nx = nvt.ny = nvt.nz = 0.0f;
			sscanf(chline, "%s %f %f %f", id, &nvt.nx, &nvt.ny, &nvt.nz);
			ncord.push_back(nvt);
		}else if(chline[0]=='f'){
			fc fct;
			fct.v1 = fct.v2 = fct.v3 = fct.t1 = fct.t2 = fct.t3 = fct.n1 = fct.n2 = fct.n3 = 0;
			sscanf(chline, "%s %u/%u/%u %u/%u/%u %u/%u/%u", id, &fct.v1, &fct.t1, &fct.n1, &fct.v2, &fct.t2, &fct.n2, &fct.v3, &fct.t3, &fct.n3);
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
		trst.t1.u = trst.t1.v = trst.t2.u = trst.t2.v = trst.t3.u = trst.t3.v = 0.0f;
		trst.n1.nx = trst.n1.ny = trst.n1.nz = trst.n2.nx = trst.n2.ny = trst.n2.nz = trst.n3.nx = trst.n3.ny = trst.n3.nz = 0.0f;

		trst.v1.x = vcord[face[i].v1-1].x;
		trst.v1.y = vcord[face[i].v1-1].y;
		trst.v1.z = vcord[face[i].v1-1].z;
		trst.v2.x = vcord[face[i].v2-1].x;
		trst.v2.y = vcord[face[i].v2-1].y;
		trst.v2.z = vcord[face[i].v2-1].z;
		trst.v3.x = vcord[face[i].v3-1].x;
		trst.v3.y = vcord[face[i].v3-1].y;
		trst.v3.z = vcord[face[i].v3-1].z;

		trst.t1.u = tcord[face[i].t1-1].u;
		trst.t1.v = tcord[face[i].t1-1].v;
		trst.t2.u = tcord[face[i].t2-1].u;
		trst.t2.v = tcord[face[i].t2-1].v;
		trst.t3.u = tcord[face[i].t3-1].u;
		trst.t3.v = tcord[face[i].t3-1].v;

		trst.n1.nx = ncord[face[i].n1-1].nx;
		trst.n1.ny = ncord[face[i].n1-1].ny;
		trst.n1.nz = ncord[face[i].n1-1].nz;
		trst.n2.nx = ncord[face[i].n2-1].nx;
		trst.n2.ny = ncord[face[i].n2-1].ny;
		trst.n2.nz = ncord[face[i].n2-1].nz;
		trst.n3.nx = ncord[face[i].n3-1].nx;
		trst.n3.ny = ncord[face[i].n3-1].ny;
		trst.n3.nz = ncord[face[i].n3-1].nz;

		tris.push_back(trst);
	}
}

void Draw()
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, object_distance);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glRotatef(angle2, 1.0f, 0.0f, 0.0f);

	Sleep(1);
	for(unsigned i=0; i<tris.size(); i++){
		glBegin(GL_TRIANGLES);
			glTexCoord2f(tris[i].t1.u, tris[i].t1.v);
			glVertex3f(tris[i].v1.x, tris[i].v1.y, tris[i].v1.z);
			glTexCoord2f(tris[i].t2.u, tris[i].t2.v);
			glVertex3f(tris[i].v2.x, tris[i].v2.y, tris[i].v2.z);
			glTexCoord2f(tris[i].t3.u, tris[i].t3.v);
			glVertex3f(tris[i].v3.x, tris[i].v3.y, tris[i].v3.z);
		glEnd();		
	}
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &cube_texture);

	glBindTexture(GL_TEXTURE_2D, cube_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SimpleImage texPNG("cs148-cube.png");
	int w = texPNG.GetWidth();
	int h = texPNG.GetHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, texPNG.GetData());

	LoadObj();
}

void DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
    glutCreateWindow("CS148 Assignment 4 Part 3");

    init();

	glutDisplayFunc(DisplayCallback);
	glutReshapeFunc(ReshapeCallback);
	glutIdleFunc(DisplayCallback);
	glutKeyboardFunc(KeyboardPressedCallback);
	glutKeyboardUpFunc(KeyboardUpCallback);

    glutMainLoop();
    return 0;
}
