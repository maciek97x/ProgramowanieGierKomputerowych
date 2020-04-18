#include <ctime>
#include <iostream>
#include <cmath>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "shaderLoader.h"

GLuint vao;
GLuint shaderProgram;

void init() {
	Core::ShaderLoader shaderLoader;
	shaderProgram = shaderLoader.CreateProgram("shader.vert", "shader.geom", "shader.frag");

	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	glBindVertexArray(0);
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(shaderProgram);

	glBindVertexArray(vao);

	glDrawArrays(GL_POINTS, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);

	glutSwapBuffers();
}

void idle() {
	glutPostRedisplay();
}

void shutdown() {

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);

	glutCreateWindow("GS");
	glEnable(GL_DEPTH_TEST);
	glewInit();

	init();

	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();
	glutMainLoop();
	return 0;
}