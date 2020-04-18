#include "Scene.h"

Scene::Scene() {
}

Scene::~Scene() {
}

int Scene::addRenderable(Renderable * object) {
    renderables_[nextRenderableId_] = object;
    return nextRenderableId_++;
}

void Scene::removeRenderable(int id) {
    renderables_.erase(id);
}

void Scene::update(float time) {
    for (auto &renderable : renderables_)
        renderable.second->update(time);
}
void Scene::render(float time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (!camera_) return;

    RenderData data;
    data.viewProjMatrix = camera_->getViewProjMatrix();
	data.viewMatrix = camera_->getViewMatrix();
	data.projMatrix = camera_->getProjectionMatrix();
	data.cameraPos = camera_->getPosition();
	data.skyboxTexture = skybox_->getTexture();
	data.time = time;

	skybox_->render(data);

    for (auto &renderable : renderables_)
        renderable.second->render(data);

    glutSwapBuffers();
	printf("\r%f  ", time);
}

void Scene::setCamera(Camera *camera) {
    camera_ = camera;
}
void Scene::setSkybox(SkyBox *skybox) {
	skybox_ = skybox;
}

