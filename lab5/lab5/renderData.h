#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

struct RenderData
{
    glm::mat4 viewProjMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
    glm::vec3 lightSource;
};

