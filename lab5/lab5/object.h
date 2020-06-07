#pragma once
#include "Renderable.h"
#include <soil.h>
#include <functional>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "sphereCollider.h"

typedef glm::mat4(*matrixFunction)(float time);

class Object : public Renderable {
public:
	Object(const char* modelPath, const char* texturePath, const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename);
	Object(const char* modelPath, const char* texturePath, const char* vertexShaderFilename, const char* fragmentShaderFilename);
	virtual ~Object();

	void setModelMatrix(glm::mat4 const& matrix);
	void setSize(float size);
	glm::mat4 getModelMatrix() const { return modelMatrix_; }
	glm::vec3 getPosition();

	void setCollider(SphereCollider* collider);
	void setPhysical(bool physical);
	bool getPhysical();
	void setMass(float size);
	float getMass();

	void addForce(glm::vec3 force);
	glm::vec3 getVelocity();
	void setVelocity(glm::vec3 velocity);
	glm::vec3 getAcceleration();
	void setAcceleration(glm::vec3 acceleration);

	void collide(Object* other);

	void update(float time);
	void updatePhysics(float time);
	void render(RenderData& data);

private:
	void init(const char* modelPath, const char* texturePath, const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename);

	int faceCount_;
	bool textured;

	glm::mat4 modelMatrix_;
	SphereCollider* collider_;
	float mass_;
	glm::vec3 velocity_;
	glm::vec3 acceleration_;
	glm::quat angularVelocity_;
	bool physical_;

	GLuint texture_;
	GLuint program_;
	GLuint vertexArray_;
	GLuint vertexBuffer_;
	GLuint vertexIndexBuffer_;
};

