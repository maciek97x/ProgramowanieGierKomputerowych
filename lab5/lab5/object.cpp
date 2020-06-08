#include "object.h"
#include "ShaderLoader.h"
#include "objload.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Object::Object(const char * modelPath, const char* texturePath, const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename) {
	init(modelPath, texturePath, vertexShaderFilename, geometryShaderFilename, fragmentShaderFilename);
}

Object::Object(const char * modelPath, const char* texturePath, const char* vertexShaderFilename, const char* fragmentShaderFilename) {
	init(modelPath, texturePath, vertexShaderFilename, NULL, fragmentShaderFilename);
}

void Object::init(const char *modelPath, const char* texturePath, const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename) {
	obj::Model model = obj::loadModelFromFile(modelPath);
	faceCount_ = model.faces["default"].size();

	position_ = glm::vec3();
	rotation_ = glm::identity<glm::quat>();
	scale_ = glm::vec3();
	modelMatrix_ = glm::identity<glm::mat4>();
	angularVelocity_ = glm::identity<glm::quat>();
	velocity_ = glm::vec3();
	acceleration_ = glm::vec3();
	mass_ = 1.0f;

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCount_ * sizeof(unsigned short), model.faces["default"].data(), GL_STATIC_DRAW);

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

	if (texturePath == NULL) {
		textured = false;
	}
	else {
		textured = true;
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
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
void Object::setMass(float mass) {
	mass_ = mass;
}

void Object::setPosition(glm::vec3 position) {
	position_ = position;
}

void Object::setRotation(glm::quat rotation) {
	rotation_ = rotation;
}

void Object::setScale(glm::vec3 scale) {
	scale_ = scale;
}

glm::vec3 Object::getPosition() {
	return position_;
}

void Object::setCollider(SphereCollider* collider) {
	collider_ = collider;
	collider->setModelMatrix(&modelMatrix_);
}

void Object::setPhysical(bool physical) {
	physical_ = physical;
}

float Object::getMass() {
	return mass_;
}

bool Object::getPhysical() {
	return physical_;
}

void Object::addForce(glm::vec3 force) {
	acceleration_ += force / mass_;
}
glm::vec3 Object::getVelocity() {
	return velocity_;
}
void Object::setVelocity(glm::vec3 velocity) {
	velocity_ = velocity;
}
glm::vec3 Object::getAcceleration() {
	return acceleration_;
}
void Object::setAcceleration(glm::vec3 acceleration) {
	acceleration_ = acceleration;
}

void Object::collide(Object* other) {
	if (collider_->collides(other->collider_)) {
		if (physical_ && other->physical_) {
			glm::vec3 delta = getPosition() - other->getPosition();

			// minimum translation distance to push balls apart after intersecting
			float d = glm::length(delta);

			glm::vec3 mtd = glm::vec3(delta);
			mtd *= (collider_->getRadius() + other->collider_->getRadius() - d) / d;

			// inverse mass quantities
			float im1 = 1 / mass_;
			float im2 = 1 / other->mass_;

			// push-pull them apart based off their mass
			modelMatrix_ = glm::translate(modelMatrix_, mtd * (im1 / (im1 + im2)));

			other->modelMatrix_ = glm::translate(other->modelMatrix_, -mtd * (im1 / (im1 + im2)));

			// impact speed
			glm::vec3 v = velocity_;
			v -= other->velocity_;

			glm::vec3 mtd_normalized = mtd / glm::length(mtd);
			float vn = glm::dot(v, mtd_normalized);

			// sphere intersecting but moving away from each other already
			if (vn > 0.0f) return;

			// collision impulse
			float i = -2.0f * vn / (im1 + im2);

			glm::vec3 impulse = mtd_normalized * i;

			// change in momentum
			velocity_ += impulse * im1;
			other->velocity_ -= impulse * im2;

			// change angular velocity after collision
			glm::vec3 deltaNormalized = delta / glm::length(delta);
			glm::vec3 deltaVelocity = other->velocity_ - velocity_;
			glm::vec3 deltaVelocityNormalized = deltaVelocity / glm::length(deltaVelocity);

			glm::vec3 rotationAxis = glm::cross(deltaNormalized, deltaVelocityNormalized);
			rotationAxis /= glm::length(rotationAxis);
			float angle = glm::length(deltaVelocity);

			angularVelocity_ *= glm::angleAxis(angle * i / im1, rotationAxis);
			other->angularVelocity_ *= glm::angleAxis(-angle * i / im2, rotationAxis);
		}
		else if (physical_) {

		}
	}
}

void Object::update(float time) {}

void Object::updatePhysics(float time) {
	if (physical_) {
		if (modelMatrix_[3][0] - collider_->getRadius() < -10 && velocity_[0] < 0) {
			modelMatrix_[3][0] = collider_->getRadius() - 10;
			velocity_[0] = -velocity_[0];
		} else if (modelMatrix_[3][0] - collider_->getRadius() > 10 && velocity_[0] > 0) {
			modelMatrix_[3][0] = collider_->getRadius() + 10;
			velocity_[0] = -velocity_[0];
		}
		if (modelMatrix_[3][2] - collider_->getRadius() < -10 && velocity_[2] < 0) {
			modelMatrix_[3][2] = collider_->getRadius() - 10;
			velocity_[2] = -velocity_[2];
		}
		else if (modelMatrix_[3][2] - collider_->getRadius() > 10 && velocity_[2] > 0) {
			modelMatrix_[3][2] = collider_->getRadius() + 10;
			velocity_[2] = -velocity_[2];
		}
		if (modelMatrix_[3][1] - collider_->getRadius() < 0 && velocity_[1] < 0) {
			modelMatrix_[3][1] = collider_->getRadius();
			velocity_[1] = -velocity_[1];
		}
		velocity_ += acceleration_ * time;

		position_ += velocity_ * time;
		rotation_ *= glm::mix(glm::identity<glm::quat>(), angularVelocity_, time);

		modelMatrix_ = glm::scale(glm::translate(glm::mat4(1.0f), position_) * glm::toMat4(rotation_), scale_);
	}
}

void Object::render(RenderData& data) {
	glUseProgram(program_);
	glBindVertexArray(vertexArray_);

	glm::mat4 modelMatrix = getModelMatrix();

	glUniformMatrix4fv(glGetUniformLocation(program_, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program_, "view"), 1, GL_FALSE, glm::value_ptr(data.viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program_, "projection"), 1, GL_FALSE, glm::value_ptr(data.projMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, data.skyboxTexture);
	glUniform1i(glGetUniformLocation(program_, "skybox"), 0);
	glUniform3fv(glGetUniformLocation(program_, "cameraPos"), 1, glm::value_ptr(data.cameraPos));
	glUniform1fv(glGetUniformLocation(program_, "time"), 1, (GLfloat*)&data.time);
	glUniform3f(glGetUniformLocation(program_, "directionalLight"), data.directionalLight.x, data.directionalLight.y, data.directionalLight.z);
	glUniform1f(glGetUniformLocation(program_, "lightsCount"), data.lightsCount);
	glUniform3fv(glGetUniformLocation(program_, "lightSourcesColors"), 32, glm::value_ptr(data.lightSourcesColors[0]));
	glUniform3fv(glGetUniformLocation(program_, "lightSourcesPositions"), 32, glm::value_ptr(data.lightSourcesPositions[0]));

	if (textured) {
		glBindTexture(GL_TEXTURE_2D, texture_);
		glUniform1i(glGetUniformLocation(program_, "tex"), 0);
	}

	glDrawElements(GL_TRIANGLES, faceCount_, GL_UNSIGNED_SHORT, (void*)0);

	if (textured) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

Object::~Object() {
}
