//==============================================================================================
//==============================================================================================
//
//         THIS IS A SKELETON CODE CLASS
//
//         DO NOT MODIFY IT !!!!!!!!!!!!!!!
//         EXCEPTION: YOU CAN MODIFY PARTS OF THE CODE REGARDING RenderData
//
//==============================================================================================
//==============================================================================================
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

int Scene::addRenderable(Renderable * object)
{
    renderables_[nextRenderableId_] = object;
    return nextRenderableId_++;
}

void Scene::removeRenderable(int id)
{
    renderables_.erase(id);
}

void Scene::update(float time)
{
    for (auto &renderable : renderables_)
        renderable.second->update(time);
}
void Scene::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (!camera_) return;

    // Modify this code if any other data needed (in case of changes in RenderData)
    RenderData data;
    data.viewProjMatrix = camera_->getViewProjMatrix();
	data.viewMatrix = camera_->getViewMatrix();
	data.projMatrix = camera_->getProjectionMatrix();

	skybox_->render(data);

    for (auto &renderable : renderables_)
        renderable.second->render(data);

    glutSwapBuffers();
}

void Scene::setCamera(Camera *camera)
{
    camera_ = camera;
}
void Scene::setSkybox(SkyBox *skybox)
{
	skybox_ = skybox;
}

