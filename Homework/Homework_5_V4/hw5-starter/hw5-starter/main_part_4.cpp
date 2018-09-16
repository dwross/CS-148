
#include "main.h"
#ifdef WIN32
#define ssize_t SSIZE_T
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cmath>
#include <float.h>
//#include <GL/glew.h> // include GLEW and new version of GL on Windows
//#include <GLFW/glfw3.h> // GLFW helper library
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

float near_field = 1.5f;
float far_field = 10000000.0f;

float object_distance = -10.0f;

float angle = 0.0f;
float angle2= 0.0f;

bool keyStateArray[256];

GLfloat lightPos0[] = {0.0f, 0.0f, 0.0f, 0.0f};

float M_amb[] = {0.0f, 0.5f, 0.0f}; // material ambient
float M_dif[] = {0.0f, 0.5f, 0.0f};	// material diffuse
float M_spc[] = {0.1f, 0.1f, 0.1f};	// material specular

float I_amb[] = {0.0f, 0.0f, 0.1f};	// light source ambient
float I_dif[] = {1.0f, 1.0f, 1.0f};	// light source diffuse
float I_spc[] = {1.0f, 1.0f, 1.0f};	// light source specular

float shine = 1.0;	// size of specular spot;

GLfloat density = 0.3;
GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};

// file locations
std::string vertexShader;
std::string fragmentShader;

SimpleShaderProgram *shader;

struct vc {
	float x;
	float y;
	float z;
};

//struct vt {
//	float u;
//	float v;
//};

struct nv {
	float nx;
	float ny;
	float nz;
};

struct fc {
	unsigned v1;
//	unsigned t1;
	unsigned n1;
	unsigned v2;
//	unsigned t2;
	unsigned n2;
	unsigned v3;
//	unsigned t3;
	unsigned n3;
};

struct trs {
	vc v1;
	vc v2;
	vc v3;
//	vt t1;
//	vt t2;
//	vt t3;
	nv n1;
	nv n2;
	nv n3;
};

//vector<vc> vcord;
//vector<vt> tcord;
//vector<nv> ncord;
//vector<fc> face;
//vector<trs> tris;
vector<vec3> vertices;
vector<vec2> uvs;
vector<vec3> normals;
unsigned long int face_size = 0;

// w - forward : s - backward : d - rotate right : a - rotate left : o - rotate up : l - rotate down
void KeyboardMovement()
{
	float delta_angle = 4.0f;
	float delta_zoom = 10000.0f;

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
		if(abs(object_distance) <= near_field + 1.5f) object_distance = -(near_field + 1.5f) ;
	}

	if(keyStateArray['s']){
		object_distance -= delta_zoom;
		if(abs(object_distance) >= far_field) object_distance = -far_field;
		if(abs(object_distance) <= near_field + 1.5f) object_distance = -(near_field + 1.5f);
	}

	if(keyStateArray['o']){
		angle2 += delta_angle;
		if(abs(angle2) >= 360.f) angle2 = 0.0f;
	}

	if(keyStateArray['l']){
		angle2 -= delta_angle;
		if(abs(angle2) >= 360.f) angle2 = 0.0f;
	}

	if(keyStateArray['q']){
		exit(0);
	}
	
	glutPostRedisplay();
}

void LoadObj(){
	vector<vec3> vcord;
	vector<vec2> tcord;
	vector<vec3> ncord;
	vector<ivec3> facev;
	vector<ivec3> facet;
	vector<ivec3> facen;

  string line;
  ifstream objfile ("..\\meshes\\Kauai_200k_Color.obj");
  char id[3];
  if (objfile.is_open())
  {
    while (getline (objfile,line)){
		char *chline = new char [line.length()+1];
		strcpy (chline, line.c_str());

		if(chline[0]=='#'){
			//skips the comment lines
		}else if(chline[0]=='v' && !((chline[1]=='t') || (chline[1]=='n'))){
			vec3 vct;
			sscanf(chline, "%s %f %f %f", id, &vct.x, &vct.y, &vct.z);
			vcord.push_back(vct);
		}else if(chline[0]=='v' && chline[1]=='t'){
			vec2 vtt;
			sscanf(chline, "%s %f %f", id, &vtt.x, &vtt.y);
			tcord.push_back(vtt);
		}else if(chline[0]=='v' && chline[1]=='n'){
			vec3 nvt;
			sscanf(chline, "%s %f %f %f", id, &nvt.x, &nvt.y, &nvt.z);
			ncord.push_back(nvt);
		}else if(chline[0]=='f'){
			ivec3 v, n, t;
//			t.x = t.y = t.z = 0.0f;
			sscanf(chline, "%s %u//%u %u//%u %u//%u", id, &v.x, &n.x, &v.y, &n.y, &v.z, &n.z);
			facev.push_back(v);
//			facet.push_back(t);
			facen.push_back(n);
			face_size++;
		}
		delete[] chline;
    }
    objfile.close();
	cout << face_size << endl;
  }else{
	  cout << "Unable to open file";
	  exit(1);
  }
	for(unsigned long int i=0; i<face_size; i++){

//		trs trst;
//		trst.v1.x = trst.v1.y = trst.v1.z = trst.v2.x = trst.v2.y = trst.v2.z = trst.v3.x = trst.v3.y = trst.v3.z = 0.0f;
//		trst.t1.u = trst.t1.v = trst.t2.u = trst.t2.v = trst.t3.u = trst.t3.v = 0.0f;
//		trst.n1.nx = trst.n1.ny = trst.n1.nz = trst.n2.nx = trst.n2.ny = trst.n2.nz = trst.n3.nx = trst.n3.ny = trst.n3.nz = 0.0f;

		vertices.push_back(vcord[facev[i].x-1]);
		vertices.push_back(vcord[facev[i].y-1]);
		vertices.push_back(vcord[facev[i].z-1]);

//		uvs.push_back(tcord[facet[i].x-1]);
//		uvs.push_back(tcord[facet[i].y-1]);
//		uvs.push_back(tcord[facet[i].z-1]);

		normals.push_back(ncord[facen[i].x-1]);
		normals.push_back(ncord[facen[i].y-1]);
		normals.push_back(ncord[facen[i].z-1]);

/*		trst.v1.x = vcord[face[i].v1-1].x;
		trst.v1.y = vcord[face[i].v1-1].y;
		trst.v1.z = vcord[face[i].v1-1].z;
		trst.v2.x = vcord[face[i].v2-1].x;
		trst.v2.y = vcord[face[i].v2-1].y;
		trst.v2.z = vcord[face[i].v2-1].z;
		trst.v3.x = vcord[face[i].v3-1].x;
		trst.v3.y = vcord[face[i].v3-1].y;
		trst.v3.z = vcord[face[i].v3-1].z;*/

/*		trst.t1.u = tcord[face[i].t1-1].u;
		trst.t1.v = tcord[face[i].t1-1].v;
		trst.t2.u = tcord[face[i].t2-1].u;
		trst.t2.v = tcord[face[i].t2-1].v;
		trst.t3.u = tcord[face[i].t3-1].u;
		trst.t3.v = tcord[face[i].t3-1].v;*/

/*		trst.n1.nx = ncord[face[i].n1-1].nx;
		trst.n1.ny = ncord[face[i].n1-1].ny;
		trst.n1.nz = ncord[face[i].n1-1].nz;
		trst.n2.nx = ncord[face[i].n2-1].nx;
		trst.n2.ny = ncord[face[i].n2-1].ny;
		trst.n2.nz = ncord[face[i].n2-1].nz;
		trst.n3.nx = ncord[face[i].n3-1].nx;
		trst.n3.ny = ncord[face[i].n3-1].ny;
		trst.n3.nz = ncord[face[i].n3-1].nz;

		tris.push_back(trst);*/
//cout << trst.v1.x << " " << trst.v1.y << " " << trst.v1.z << " " << trst.v2.x << " " << trst.v2.y << " " << trst.v2.z << " " << trst.v3.x << " " << trst.v3.y << " " << trst.v3.z << endl;

	}
}

void DrawWithShader(){
	glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glTranslatef(0.0f, 0.0f, object_distance);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glRotatef(angle2, 1.0f, 0.0f, 0.0f);

    shader->Bind();


	// lighting paramters for the per pixel lighting

	// changes color as a function of distance form the camera

/*	if(object_distance >= -13){
		M_amb[0] = M_dif[0] = ((1.0f/20.0)*object_distance) + (13.0f/20.0);
		M_amb[1] = M_dif[1] = ((-1.0f/20.0)*object_distance) - (3.0f/20.0);
	}

	if(object_distance < -13) {
		M_amb[1] = M_dif[1] = ((1.0f/20.0)*object_distance) + (23.0f/20.0);
		M_amb[2] = M_dif[2] = ((-1.0f/20.0)*object_distance) - (13.0f/20.0);
	}*/

	// sets uniform paramters to be passed to the fragment shader

	shader->SetUniform("M_amb", M_amb[0], M_amb[1], M_amb[2]);
	shader->SetUniform("M_dif", M_dif[0], M_dif[1], M_dif[2]);
	shader->SetUniform("M_spc", M_spc[0], M_spc[1], M_spc[2]);

	shader->SetUniform("I_amb", I_amb[0], I_amb[1], I_amb[2]);
	shader->SetUniform("I_dif", I_dif[0], I_dif[1], I_dif[2]);
	shader->SetUniform("I_spc", I_spc[0], I_spc[1], I_spc[2]);

	shader->SetUniform("shine", shine);

	for(unsigned long int i=0; i<(face_size*3); i = i+3){

		glBegin(GL_TRIANGLES);

/*			glNormal3f(tris[i].n1.nx, tris[i].n1.ny, tris[i].n1.nz);
			glVertex3f(tris[i].v1.x, tris[i].v1.y, tris[i].v1.z);

			glNormal3f(tris[i].n2.nx, tris[i].n2.ny, tris[i].n2.nz);
			glVertex3f(tris[i].v2.x, tris[i].v2.y, tris[i].v2.z);

			glNormal3f(tris[i].n3.nx, tris[i].n3.ny, tris[i].n3.nz);
			glVertex3f(tris[i].v3.x, tris[i].v3.y, tris[i].v3.z);*/
			
			for(unsigned long int j = i; j < i+3; j++){ 

				glNormal3f(normals[j].x, normals[j].y, normals[j].z);
				glVertex3f(vertices[j].x, vertices[j].y, vertices[j].z);
			}

		glEnd();	
	}
    shader->UnBind();
}

void DisplayCallback(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    DrawWithShader();
    glutSwapBuffers();
}

void ReshapeCallback(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, near_field, far_field);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -10.5, 0.0, 1.0, 0.0);
}

void Setup()
{

	/* get version info */
//	const GLubyte* renderer = glGetString (GL_RENDERER); /* get renderer string */
//	const GLubyte* version = glGetString (GL_VERSION); /* version as a string */
//	printf ("Renderer: %s\n", renderer);
//	printf ("OpenGL version supported %s\n", version);

    shader = new SimpleShaderProgram();
    shader->LoadVertexShader(vertexShader);
    shader->LoadFragmentShader(fragmentShader);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING); // enable lighting
    glEnable(GL_LIGHT0);

	glEnable(GL_CULL_FACE);

	LoadObj();
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

int main(int argc, char** argv){
    if(!(argc == 3)){
        printf("usage: ./hw5 <vertex shader> <fragment shader> \n");
        return 0;
    }

    vertexShader   = string(argv[1]);
    fragmentShader = string(argv[2]);

    // Initialize GLUT.
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CS148 Assignment 5");

    //
    // Initialize GLEW.
    //
#if !defined(__APPLE__) && !defined(__linux__)
    glewInit();
    if(!GLEW_VERSION_2_0) {
        printf("Your graphics card or graphics driver does\n"
               "\tnot support OpenGL 2.0, trying ARB extensions\n");

        if(!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
            printf("ARB extensions don't work either.\n");
            printf("\tYou can try updating your graphics drivers.\n"
                   "\tIf that does not work, you will have to find\n");
            printf("\ta machine with a newer graphics card.\n");
            exit(1);
        }
    }
#endif

    Setup();

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
	glutKeyboardFunc(KeyboardPressedCallback);
	glutKeyboardUpFunc(KeyboardUpCallback);
    glutIdleFunc(DisplayCallback);

    glutMainLoop();
    return 0;
}
