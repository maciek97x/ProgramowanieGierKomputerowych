#pragma once
#include <map>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "Camera.h"
#include "Renderable.h"
#include "RenderData.h"
#include "Skybox.h"
using namespace std;

// Class responsible for rendering scene, setting camera and observing input
class Scene
{
public:
    Scene();
    virtual ~Scene();

    //adds object to render loop, returns its id
    // object - pointer to object
    int addRenderable(Renderable* renderable);

    //removes object from render loop found by its id
    void removeRenderable(int id);
    
    // update scene
    void update(float time);

    // render scene
    void render();

    //sets scene camera
    void setCamera(Camera* camera);

	void setSkybox(SkyBox* skybox);

private:
    map<unsigned int, Renderable*> renderables_;
    Camera* camera_ = nullptr;
	SkyBox* skybox_ = nullptr;
    unsigned int nextRenderableId_ = 1;
    unsigned int nextInputId_ = 1;
};

