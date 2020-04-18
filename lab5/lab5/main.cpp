#include <ctime>
#include <iostream>
#include <cmath>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"


#include "camera.h"
#include "cameraController.h"
#include "scene.h"
#include "skybox.h"
#include "object.h"
#include "shaderLoader.h"

Camera camera;
CameraController cameraController;
Scene scene;
SkyBox skybox;

vector<std::string> skyboxFaces =
{
	"textures/skybox/right.png",
	"textures/skybox/left.png",
	"textures/skybox/top.png",
	"textures/skybox/bottom.png",
	"textures/skybox/front.png",
	"textures/skybox/back.png"
};

void init() {
	cameraController.setCamera(&camera);
	scene.setCamera(&camera);
	camera.setViewProperties(glm::vec3(-6.0f, 3.0f, -3.0f), glm::quat(glm::vec3(0.0f, glm::half_pi<float>(), 0.0f)));

	skybox.init(skyboxFaces);
	scene.setSkybox(&skybox);
}

void renderScene() {
	static double prevTime = 0;
	double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	double dtime = time - prevTime;
	prevTime = time;

	scene.update(time);
	scene.render();
}

void idle() {
	glutPostRedisplay();
}

void shutdown() {

}

void mouse(int button, int state, int x, int y)
{
	cameraController.mouseInput(button, state, x, y);
}

void motion(int x, int y)
{
	cameraController.mouseInput(-1, -1, x, y);
}

void keyboard(unsigned char key, int x, int y) {
	cameraController.keyboardInput(true, key, x, y);
}

void keyboardUp(unsigned char key, int x, int y) {
	cameraController.keyboardInput(false, key, x, y);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);

	glutCreateWindow("GL");
	glEnable(GL_DEPTH_TEST);
	glewInit();

	init();

	glutDisplayFunc(renderScene);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();
	glutMainLoop();
	return 0;
}