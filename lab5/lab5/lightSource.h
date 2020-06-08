#pragma once
#include "Renderable.h"
#include "camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <functional>

typedef glm::mat4(*matrixFunction)(float time);

class LightSource : public Renderable {
public:
	LightSource(glm::vec3 color, const char* vertexShaderFilename, const char* fragmentShaderFilename);
	virtual ~LightSource();

	void setPosition(glm::vec3 position);
	void setRotation(glm::quat rotation);
	void setSize(float size);
	void setCamera(Camera *camera);
	glm::mat4 getModelMatrix() const { return modelMatrix_; }
	glm::vec3 getPosition();
	glm::vec3 getColor();

	void setMatrixFunction(std::function<glm::mat4(float)> func);

	void update(float time);

	void render(RenderData& data);

private:
	void init(glm::vec3 color, const char* vertexShaderFilename, const char* fragmentShaderFilename);

	glm::vec3 position_;
	glm::quat rotation_;
	float size_;
	glm::mat4 modelMatrix_;
	std::function<glm::mat4(float)> matrixFunction_;
	GLuint program_;
	glm::vec3 color_;

	Camera* camera_;
};