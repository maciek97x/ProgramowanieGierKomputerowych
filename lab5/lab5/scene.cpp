#include "Scene.h"

Scene::Scene(float gravity) {
	gravity_ = gravity;
}

Scene::~Scene() {
}

int Scene::addObject(Object * object) {
	if (object->getPhysical()) {
		object->addForce(glm::vec3(0.0f, -gravity_ * object->getMass(), 0.0f));
	}
    objects_[nextObjectId_] = object;

    return nextObjectId_++;
}

void Scene::removeObject(int id) {
    objects_.erase(id);
}

int Scene::addLightSource(LightSource * lightSource) {
	lightSources_[nextLightSourceId_] = lightSource;
	lightSource->setCamera(camera_);
	return nextLightSourceId_++;
}

void Scene::removeLightSource(int id) {
	lightSources_.erase(id);
}

void Scene::setDirectionalLight(glm::vec3 directionalLight)
{
	directionalLight_ = directionalLight;
}

glm::vec3 Scene::getDirectionalLight() const
{
	return directionalLight_;
}

void Scene::update(float time) {
	for (auto &object : objects_) {
		object.second->update(time);
	}
	for (auto &lightSource : lightSources_) {
		lightSource.second->update(time);
	}
}

void Scene::updatePhysics(float time) {
	for (auto &object : objects_) {
		object.second->updatePhysics(time);
	}
	for (auto &object_1 : objects_) {
		for (auto &object_2 : objects_) {
			if (object_1.first != object_2.first) {
				Object* obj_1 = object_1.second;
				Object* obj_2 = object_2.second;
				obj_1->collide(obj_2);
			}
		}
	}
}

void Scene::render(float time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (!camera_) return;

    RenderData data;
	data.directionalLight = directionalLight_;
    data.viewProjMatrix = camera_->getViewProjMatrix();
	data.viewMatrix = camera_->getViewMatrix();
	data.projMatrix = camera_->getProjectionMatrix();
	data.cameraPos = camera_->getPosition();
	data.skyboxTexture = skybox_->getTexture();
	data.time = time;
	data.lightSourcesColors = (glm::vec3*)malloc(lightSources_.size() * sizeof(glm::vec3));
	data.lightSourcesPositions = (glm::vec3*)malloc(lightSources_.size() * sizeof(glm::vec3));

	int i = 0;
	for (auto &lightSource : lightSources_) {
		data.lightSourcesColors[i] = lightSource.second->getColor();
		data.lightSourcesPositions[i] = lightSource.second->getPosition();
		++i;
	}

	data.lightsCount = i;

	skybox_->render(data);

	for (auto &object : objects_) {
		object.second->render(data);
	}
	for (auto &lightSource : lightSources_) {
		lightSource.second->render(data);
	}

    glutSwapBuffers();
}

void Scene::setCamera(Camera *camera) {
    camera_ = camera;
}
void Scene::setSkybox(SkyBox *skybox) {
	skybox_ = skybox;
}

