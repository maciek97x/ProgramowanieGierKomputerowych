#include "sphereCollider.h"

SphereCollider::SphereCollider(float radius) {
	radius_ = radius;
}

SphereCollider::~SphereCollider() {

}

float SphereCollider::getRadius() {
	return radius_;
}

glm::mat4* SphereCollider::getModelMatrix() {
	return modelMatrix_;
}

void SphereCollider::setModelMatrix(glm::mat4* modelMatrix) {
	modelMatrix_ = modelMatrix;
}

bool SphereCollider::collides(SphereCollider* other) {
	glm::vec3 this_position = glm::vec3((*modelMatrix_)[3]);
	glm::vec3 other_position = glm::vec3((*(other->modelMatrix_))[3]);

	float distance = glm::length(this_position - other_position);

	return abs(radius_ - other->radius_) < distance && distance < radius_ + other->radius_;
}
