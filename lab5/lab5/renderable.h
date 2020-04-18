#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "RenderData.h"

class Renderable {
public:
    Renderable() {}
    virtual ~Renderable() {}

    // set model matrix
    // (e.g. called by physics simulation when this renderable
    // is set as userData for the physical body)
    // matrix - matrix to be set
    virtual void setModelMatrix(glm::mat4 const& matrix) = 0;

    // get model matrix
    virtual glm::mat4 getModelMatrix() const = 0;

    // update (called every frame)
    // time - time of simulation
    virtual void update(float time) = 0;

    // render (called every frame)
    // data - data for rendering
    virtual void render(RenderData& data) = 0;
};

