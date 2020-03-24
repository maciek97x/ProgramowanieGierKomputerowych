#include "cameraController.h"
#include "GL/freeglut.h"

CameraController::CameraController() {
	moveSpeed_ = 20.0f;
	rotationSpeed_ = glm::radians(10.0f);
	glm::vec3 doffset_(0, 0, 0);
	float dyaw_ = 0.0f;
}

CameraController::~CameraController() {
}

void CameraController::setCamera(Camera *camera) {
	camera_ = camera;
}

void CameraController::keyboardInput(bool keyDown, unsigned char key, int x, int y) {
	glm::vec3 offset(0, 0, 0);
	float yaw = 0;

	if (keyDown)
	{
		switch (key)
		{
		case 'd':
			doffset_.x = 1.0f;
			break;
		case 'a':
			doffset_.x = -1.0f;
			break;
		case 'r':
			doffset_.y = 1.0f;
			break;
		case 'f':
			doffset_.y = -1.0f;
			break;
		case 'w':
			doffset_.z = 1.0f;
			break;
		case 's':
			doffset_.z = -1.0f;
			break;
		case 'q':
			dyaw_ = 1.0f;
			break;
		case 'e':
			dyaw_ = -1.0f;;
			break;
		}
	}
	else {
		switch (key)
		{
		case 'd':
		case 'a':
			doffset_.x = 0.0f;
			break;
		case 'r':
		case 'f':
			doffset_.y = 0.0f;
			break;
		case 'w':
		case 's':
			doffset_.z = 0.0f;
			break;
		case 'q':
		case 'e':
			dyaw_ = 0.0f;;
			break;
		}
	}
}

void CameraController::mouseInput(int button, int state, int x, int y) {
	if (!camera_) return;

	if (lastX_ < 0 || state == GLUT_DOWN) {
		lastX_ = x, lastY_ = y;
	}
	if (x == lastX_ && y == lastY_) return;

	float scale = 0.05f;
	float angy = -(x - lastX_) * rotationSpeed_ * scale;
	float angx = -(y - lastY_) * rotationSpeed_ * scale;
	glm::quat rotx = glm::angleAxis(angx, camera_->getSide());
	glm::quat roty = glm::angleAxis(angy, camera_->getUp());
	rotate(roty * rotx);
	lastX_ = x;
	lastY_ = y;
}

void CameraController::step(float stepTime) {
	glm::vec3 offset(0, 0, 0);
	float yaw = 0;

	offset.x += doffset_.x * moveSpeed_ * stepTime;
	offset.y += doffset_.y * moveSpeed_ * stepTime;
	offset.z += doffset_.z * moveSpeed_ * stepTime;
	yaw += dyaw_ * rotationSpeed_ * 10 * stepTime;

	if (offset != glm::vec3(0, 0, 0)) translate(offset);
	if (yaw != 0) rotate(glm::angleAxis(yaw, glm::vec3(0, 1, 0)));
}

void CameraController::translate(glm::vec3 const & offset) {
	if (!camera_) return;

	glm::vec3 pos = camera_->getPosition()
		+ camera_->getSide() * offset.x
		+ camera_->getDirection() * offset.z
		+ camera_->getUp() * offset.y;

	camera_->setViewProperties(pos, camera_->getDirection(), camera_->getSide());
}

void CameraController::rotate(glm::quat const & rotation) {
	if (!camera_) return;

	glm::vec3 dir = rotation * camera_->getDirection();
	glm::vec3 side = glm::cross(dir, glm::vec3(0, 1, 0));
	camera_->setViewProperties(camera_->getPosition(), dir, side);
}
