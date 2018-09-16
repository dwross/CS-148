/*
 * Assignent 4 Part 1 - David Ross (SCPD)
 */

// freeglut includes all of the other OpenGL headers
#include <GL/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void display(void)
{
// clear all pixels
    glClear(GL_COLOR_BUFFER_BIT);

// draws a series of cardinal coldered 
// rectangular polygons to form a large "S"
    glColor3f(130.0f/255.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
        glVertex3f(0.05f, 0.10f, 0.0f);
        glVertex3f(0.95f, 0.10f, 0.0f);
        glVertex3f(0.95f, 0.15f, 0.0f);
        glVertex3f(0.05f, 0.15f, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(0.90f, 0.15f, 0.0f);
        glVertex3f(0.95f, 0.10f, 0.0f);
        glVertex3f(0.95f, 0.475f, 0.0f);
        glVertex3f(0.90f, 0.475f, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(0.05f, 0.475f, 0.0f);
        glVertex3f(0.95f, 0.475f, 0.0f);
        glVertex3f(0.95f, 0.525f, 0.0f);
        glVertex3f(0.05f, 0.525f, 0.0f);
    glEnd();
	glBegin(GL_POLYGON);
        glVertex3f(0.05f, 0.525f, 0.0f);
        glVertex3f(0.10f, 0.525f, 0.0f);
        glVertex3f(0.10f, 0.90f, 0.0f);
        glVertex3f(0.05f, 0.90f, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(0.10f, 0.85f, 0.0f);
        glVertex3f(0.95f, 0.85f, 0.0f);
        glVertex3f(0.95f, 0.90f, 0.0f);
        glVertex3f(0.10f, 0.90f, 0.0f);
    glEnd();

// draws a green-brown box for a tree trunk
    glColor3f(102.0f/255.0f, 102.0f/255.0f, 0.0f);
    glBegin(GL_POLYGON);
        glVertex3f(0.425f, 0.075f, 0.0f);
        glVertex3f(0.575f, 0.075f, 0.0f);
        glVertex3f(0.575f, 0.225f, 0.0f);
        glVertex3f(0.425f, 0.225f, 0.0f);
    glEnd();

// draws a green triangle for a tree top
    glColor3f(0.0f, 204.0f/255.0f, 0.0f);
    glBegin(GL_POLYGON);
        glVertex3f(0.333f, 0.225f, 0.0f);
        glVertex3f(0.666f, 0.225f, 0.0f);
        glVertex3f(0.500f, 0.925f, 0.0f);
    glEnd();

// don't wait!
// starts processing buffered OpenGL routines
    glFlush();
}

void init(void)
{
// selects clearing (background) color
    glClearColor(0.0, 0.0, 0.0, 0.0);

// initializes viewing values
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS148 Assignment 4 Part 1");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

