#include <GL/GLUT.h>
#include<iostream>
#include<cstdlib>

#include <cmath>
#include <cstdio>

#define PI 3.14159265f

char title[] = "Zadanie 1";
int windowWidth = 640;
int windowHeight = 480;
int windowPosX = 100;
int windowPosY = 100;

struct PointF {
	GLfloat x;
	GLfloat y;
};

PointF elipsePos = { 0.0f, 0.0f };
PointF elipseRadius = { 0.05f, 0.05f };
PointF elipseDir = { 1.0f, 1.0f };
PointF elipseVelocity = { 0.005f, 0.005f };

GLdouble left, right, bottom, top;

int refreshMillis = 10;

void initGL() {
	glClearColor(0.75f, 0.75f, 0.75f, 1.0);
}

void display() {
	elipsePos.x += elipseDir.x * elipseVelocity.x;
	elipsePos.y += elipseDir.y * elipseVelocity.y;

	int a = 0;
	if (elipsePos.x - elipseRadius.x <= left) {
		elipsePos.x = left + elipseRadius.x;
		elipseDir.x = 1;
		a += 1;
	}
	if (elipsePos.x + elipseRadius.x >= right) {
		elipsePos.x = right - elipseRadius.x;
		elipseDir.x = -1;
		a += 2;
	}
	if (elipsePos.y - elipseRadius.y <= bottom) {
		elipsePos.y = bottom + elipseRadius.y;
		elipseDir.y = 1;
		a += 4;
	}
	if (elipsePos.y + elipseRadius.y >= top) {
		elipsePos.y = top - elipseRadius.y;
		elipseDir.y = -1;
		a += 8;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(left, bottom / 2);
	glVertex2f(right, bottom / 2);
	glVertex2f(right, bottom / 4);
	glVertex2f(left, bottom / 4);

	glEnd();

	glLoadIdentity();
	glTranslatef(elipsePos.x, elipsePos.y, 0.0f);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.5f, 0.75f);
	glVertex2f(0.0f, 0.0f);

	int numSegments = 100;
	GLfloat angle;
	for (int i = 0; i <= numSegments; i++)
	{
		angle = i * 2.0f * PI / numSegments;
		glVertex2f(cos(angle) * elipseRadius.x, sin(angle) * elipseRadius.y);
	}

	glEnd();

	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.5f, 0.0f);
	glVertex2f(left, top/2);
	glVertex2f(right, top/2);
	glVertex2f(right, top/4);
	glVertex2f(left, top/4);


	glEnd();
	glutSwapBuffers();
}


void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat) width / height;

	glViewport(0, 0, width, height);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width <= height) {
		left = -1.0;
		right = 1.0;
		bottom = -1.0 / aspect;
		top = 1.0 / aspect;
	}
	else {
		left = -1.0 * aspect;
		right = 1.0 * aspect;
		bottom = -1.0;
		top = 1.0;
	}
	gluOrtho2D(left, right, bottom, top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void specialInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		elipseVelocity.x *= 1.2;
		break;
	case GLUT_KEY_LEFT:
		elipseVelocity.x /= 1.2;
		break;
	case GLUT_KEY_UP:
		elipseVelocity.y *= 1.2;
		break;
	case GLUT_KEY_DOWN:
		elipseVelocity.y /= 1.2;
		break;
	}
	glutPostRedisplay();
}

void timer(int value){
	glutPostRedisplay();
	glutTimerFunc(refreshMillis, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutCreateWindow(title);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialInput);
	glutTimerFunc(0, timer, 0);
	initGL();
	glutMainLoop();
	return 0;
}