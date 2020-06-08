#pragma once
#include <map>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "camera.h"
#include "object.h"
#include "lightSource.h"
#include "renderData.h"
#include "skybox.h"
using namespace std;

// Class responsible for rendering scene and setting camera
class Scene {
public:
    Scene(float gravity);
    virtual ~Scene();

    // adds object to render loop, returns its id
    // object - pointer to object
    int addObject(Object* object);

    // removes object from render loop found by its id
    void removeObject(int id);

	// same as for objects
	int addLightSource(LightSource* lightSource);

	void removeLightSource(int id);

	// sets directional light
	void setDirectionalLight(glm::vec3 directionalLight);
	glm::vec3 getDirectionalLight() const;

    // update scene
    void update(float time);

	// update scene physics
	void updatePhysics(float time);

    // render scene
    void render(float time);

    // sets scene camera
    void setCamera(Camera* camera);

	void setSkybox(SkyBox* skybox);

private:
    map<unsigned int, Object*> objects_;
	map<unsigned int, LightSource*> lightSources_;
    Camera* camera_ = nullptr;
	SkyBox* skybox_ = nullptr;
    unsigned int nextObjectId_ = 1;
	unsigned int nextLightSourceId_ = 1;
	float gravity_ = 1.0f;
	glm::vec3 directionalLight_;
};

