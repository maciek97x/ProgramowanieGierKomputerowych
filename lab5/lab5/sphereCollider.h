#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class PlaneCollider;

class SphereCollider{
public:
	SphereCollider(float radius);
	virtual ~SphereCollider();

	float getRadius();
	glm::mat4* getModelMatrix();
	void setModelMatrix(glm::mat4* modelMatrix);

	bool collides(SphereCollider* other);

private:
	glm::mat4* modelMatrix_;
	float radius_ = 0;
};