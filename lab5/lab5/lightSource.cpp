#include "lightSource.h"
#include "ShaderLoader.h"


LightSource::LightSource(glm::vec3 color, const char* vertexShaderFilename, const char* fragmentShaderFilename) {
	init(color, vertexShaderFilename, fragmentShaderFilename);
}

void LightSource::init(glm::vec3 color, const char* vertexShaderFilename,  const char* fragmentShaderFilename) {
	Core::ShaderLoader shaderLoader;

	program_ = shaderLoader.CreateProgram(
		vertexShaderFilename,
		fragmentShaderFilename);

	modelMatrix_ = glm::identity<glm::mat4>();
	position_ = glm::vec3(0.0f);
	rotation_ = glm::identity<glm::quat>();
	size_ = 1.0f;
	color_ = color;

	matrixFunction_ = [](float time) {
		return glm::identity<glm::mat4>();
	};
}

void LightSource::setCamera(Camera *camera) {
	camera_ = camera;
}

glm::vec3 LightSource::getPosition() {
	return glm::vec3(modelMatrix_[3]);
}
glm::vec3 LightSource::getColor() {
	return color_;
}

void LightSource::setMatrixFunction(std::function<glm::mat4(float)> func) {
	matrixFunction_ = func;
}

void LightSource::setSize(float size) {
	size_ = size;
}

void LightSource::setPosition(glm::vec3 position) {
	position_ = position;
}

void LightSource::setRotation(glm::quat rotation) {
	rotation_ = rotation;
}

void LightSource::update(float time) {
	glm::mat4 externalTransform;
	if (matrixFunction_) {
		externalTransform = matrixFunction_(time);
	}
	glm::vec3 direction = getPosition() - camera_->getPosition();
	direction /= glm::length(direction);
	rotation_ = glm::quatLookAt(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix_ = glm::scale(glm::translate(glm::mat4(1.0f), position_) * externalTransform * glm::toMat4(rotation_), glm::vec3(size_));
}

void LightSource::render(RenderData& data) {
	glUseProgram(program_);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 modelViewProjMatrix = data.viewProjMatrix * getModelMatrix();

	glUniformMatrix4fv(glGetUniformLocation(program_, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&modelViewProjMatrix);

	glUniform3f(glGetUniformLocation(program_, "color"), color_.x, color_.y, color_.z);

	float vert_pos[] = {
		-0.5, -0.5, 0,
		-0.5, 0.5, 0,
		0.5, 0.5, 0,
		-0.5, -0.5, 0,
		0.5, 0.5, 0,
		0.5, -0.5, 0,
	};
	float vert_texc[] = {
		0, 1,
		0, 0,
		1, 0,
		0, 1,
		1, 0,
		1, 1,
	};

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, vert_pos);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, vert_texc);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);

	glUseProgram(0);
}

LightSource::~LightSource() {}