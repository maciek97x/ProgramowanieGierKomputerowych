
#include "object.h"
#include "ShaderLoader.h"
#include "objload.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Object::Object(const char * modelPath, const char *texturePath,
	const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename)
{
	init(modelPath, texturePath, vertexShaderFilename, geometryShaderFilename, fragmentShaderFilename);
}

void Object::init(const char * modelPath, const char *texturePath,
	const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename)
{
	obj::Model model = obj::loadModelFromFile(modelPath);
	faceCount_ = model.faces["default"].size();

	Core::ShaderLoader shaderLoader;

	if (geometryShaderFilename != NULL) {
		program_ = shaderLoader.CreateProgram(
			vertexShaderFilename,
			geometryShaderFilename,
			fragmentShaderFilename);
	}
	else {
		program_ = shaderLoader.CreateProgram(
			vertexShaderFilename,
			fragmentShaderFilename);
	}

	glGenVertexArrays(1, &vertexArray_);
	glBindVertexArray(vertexArray_);
	glGenBuffers(1, &vertexIndexBuffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		faceCount_ * sizeof(unsigned short),
		model.faces["default"].data(), GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBuffer_);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
	size_t vsize = model.vertex.size() * sizeof(float);
	size_t nsize = model.normal.size() * sizeof(float);
	size_t tsize = model.texCoord.size() * sizeof(float);
	size_t bufsize = vsize + nsize + tsize;
	glBufferData(GL_ARRAY_BUFFER, bufsize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, model.vertex.data());
	glBufferSubData(GL_ARRAY_BUFFER, vsize, nsize, model.normal.data());
	if (tsize > 0) {
		glBufferSubData(GL_ARRAY_BUFFER, vsize + nsize, tsize, model.texCoord.data());
	}

	auto glPos = glGetAttribLocation(program_, "vertexPosition");
	glEnableVertexAttribArray(glPos);
	glVertexAttribPointer(glPos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	auto glNor = glGetAttribLocation(program_, "vertexNormal");
	glEnableVertexAttribArray(glNor);
	glVertexAttribPointer(glNor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vsize));

	auto glUV = glGetAttribLocation(program_, "vertexTexCoord");
	glEnableVertexAttribArray(glUV);
	glVertexAttribPointer(glUV, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vsize + nsize));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	texture_ = SOIL_load_OGL_texture(
		texturePath,
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // sposob filtrowania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Object::setMatrixFunction(std::function<glm::mat4(float)> func)
{
	matrixFunction_ = func;
}

void Object::setModelMatrix(glm::mat4 const& matrix) {
	localModelMatrix_ = matrix;
}

void Object::update(float time)
{
	glm::mat4 externalTransform;
	if (matrixFunction_) {
		externalTransform = matrixFunction_(time);
	}
	modelMatrix_ = externalTransform * localModelMatrix_;
}

void Object::render(RenderData& data)
{
	glUseProgram(program_);
	glBindVertexArray(vertexArray_);

	glm::mat4 modelViewProjMatrix = data.viewProjMatrix * getModelMatrix();
	glm::mat4 modelMatrix = getModelMatrix();

	glUniformMatrix4fv(glGetUniformLocation(program_,
		"modelViewProjectionMatrix"), 1, GL_FALSE,
		(float*)&modelViewProjMatrix);

	glUniformMatrix4fv(glGetUniformLocation(program_,
		"modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	glUniform3f(glGetUniformLocation(program_, "lightSource"),
		data.lightSource.x, data.lightSource.y, data.lightSource.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glUniform1i(glGetUniformLocation(program_, "tex"), 0);

	glDrawElements(GL_TRIANGLES, faceCount_, GL_UNSIGNED_SHORT, (void*)0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

Object::~Object()
{
}
