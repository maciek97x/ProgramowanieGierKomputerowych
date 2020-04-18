#pragma once
#include "Renderable.h"
#include "soil.h"
#include<functional>

typedef glm::mat4(*matrixFunction)(float time);

class Object : public Renderable {
public:
	Object(const char* modelPath, const char* texturePath, const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename);
	virtual ~Object();

	// set function to be called to update the model matrix
	void setMatrixFunction(std::function<glm::mat4(float)> func);

	void setModelMatrix(glm::mat4 const& matrix);
	glm::mat4 getModelMatrix() const { return modelMatrix_; }

	void update(float time);
	void render(RenderData& data);

private:
	void init(const char* modelPath, const char* texturePath, const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename);

	int faceCount_;
	bool textured;

	glm::mat4 localModelMatrix_, modelMatrix_;

	GLuint texture_;
	GLuint program_;
	GLuint vertexArray_;
	GLuint vertexBuffer_;
	GLuint vertexIndexBuffer_;
	std::function<glm::mat4(float)> matrixFunction_;
};

