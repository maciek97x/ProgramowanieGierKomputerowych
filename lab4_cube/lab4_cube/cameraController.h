#pragma once
#include "Camera.h"

// Camera controller
// Use WSAD to move camera, QE to rotate it
// Use mouse click + drag to rotate
class CameraController {
public:
	CameraController();

	void setCamera(Camera *camera);

	void keyboardInput(bool keyDown, unsigned char key, int x, int y);
	void mouseInput(int button, int state, int x, int y);
	void step(float stepTime);
	virtual ~CameraController();

private:
	int lastX_ = -1, lastY_ = -1;
	float moveSpeed_, rotationSpeed_, dyaw_;
	glm::vec3 doffset_;
	Camera* camera_ = nullptr;

	void translate(glm::vec3 const &offset);
	void rotate(glm::quat const &rotation);
};
