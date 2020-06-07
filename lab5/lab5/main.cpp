#include <ctime>
#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "camera.h"
#include "cameraController.h"
#include "scene.h"
#include "skybox.h"
#include "object.h"
#include "shaderLoader.h"
#include "sphereCollider.h"

const float gravity = 9.8;

// fixed timestep for stable and deterministic simulation
const double physicsStepTime = 1.0 / 60.0;
double physicsTimeToProcess = 0;

Camera camera;
CameraController cameraController;
Scene scene(gravity);
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
	srand(time(NULL));

	scene.setDirectionalLight(glm::normalize(glm::vec3(1.0f, 2.0f, 1.0f)));

	cameraController.setCamera(&camera);
	scene.setCamera(&camera);
	camera.setViewProperties(glm::vec3(-10.0f, -2.0f, 2.0f), glm::quat(glm::vec3(0.0f, glm::half_pi<float>(), 0.0f)));

	skybox.init(skyboxFaces);
	scene.setSkybox(&skybox);

	for (int i = 0; i < 20; ++i) {
		glm::vec3 randPosition = glm::linearRand(glm::vec3(-8.0f, 3.0f, -8.0f), glm::vec3(8.0f, 30.0f, 8.0f));
		float radius = 1.0f; // 0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.9f));

		Object *ball = new Object("models/ball.obj", "textures/soccer.png", "shaders/shader_texture.vert", "shaders/shader_texture.frag");
		ball->setSize(radius);
		ball->setMass(4 * glm::pi<float>() / 3 * pow(radius, 3));
		ball->setCollider(new SphereCollider(radius));
		ball->setPhysical(true);
		ball->setModelMatrix(glm::translate(glm::identity<glm::mat4>(), randPosition));
		scene.addObject(ball);
	}
}

void updatePhysics(float dtime)
{
	if (dtime < 1.0f)
	{
		physicsTimeToProcess += dtime;
	}
	while (physicsTimeToProcess > 0)
	{
		scene.updatePhysics(physicsStepTime);
		physicsTimeToProcess -= physicsStepTime;
	}
}

void renderScene() {
	static double prevTime = 0;
	double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	double dtime = time - prevTime;
	prevTime = time;

	cameraController.step(dtime);

	updatePhysics(dtime);
	scene.render(time);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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