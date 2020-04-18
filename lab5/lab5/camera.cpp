#include "Camera.h"
#include "GL/freeglut.h"

Camera::Camera() {
	pos_ = glm::vec3(0, 10, 20);
	dir_ = glm::vec3(0, 0, -1);
	side_ = glm::vec3(1, 0, 0);
	fovy_ = glm::radians(60.f);
	znear_ = 0.1f;
	zfar_ = 1000.f;
}

Camera::~Camera() {
}

void Camera::setViewProperties(glm::vec3 const & position, glm::vec3 const & direction, glm::vec3 const & side) {
	pos_ = position;
	dir_ = glm::normalize(direction);
	side_ = glm::normalize(side);
}

void Camera::setViewProperties(glm::vec3 const & position, glm::quat const & rotation) {
	pos_ = position;
	auto irot = glm::inverse(rotation);
	dir_ = glm::normalize(irot * glm::vec3(0, 0, -1));
	side_ = glm::normalize(irot * glm::vec3(1, 0, 0));
}

void Camera::setProjectionProperties(float fovy, float znear, float zfar) {
	fovy_ = fovy;
	znear_ = znear;
	zfar_ = zfar;
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(pos_, pos_ + dir_, getUp());
}

glm::mat4 Camera::getProjectionMatrix() const {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	return glm::perspectiveFov(fovy_, (float)width,
		(float)height, znear_, zfar_);
}

glm::mat4 Camera::getViewProjMatrix() const {
	return getProjectionMatrix() * getViewMatrix();
}
