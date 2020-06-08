#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RenderData.h"

class Renderable {
public:
    Renderable() {}
    virtual ~Renderable() {}

	virtual void setPosition(glm::vec3 position) = 0;

	virtual void setRotation(glm::quat rotation) = 0;

    // get model matrix
    virtual glm::mat4 getModelMatrix() const = 0;

    // update (called every frame)
    // time - time of simulation
    virtual void update(float time) = 0;

    // render (called every frame)
    // data - data for rendering
    virtual void render(RenderData& data) = 0;
};

