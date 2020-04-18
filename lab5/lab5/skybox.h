#pragma once
#include "Camera.h"
#include "ShaderLoader.h"
#include "soil.h"
#include "Renderable.h"
#include "objload.h"

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "RenderData.h"
#include <vector>
#include <string>

class SkyBox : public Renderable
{
public:
	SkyBox();

	~SkyBox();

    bool init(std::vector<std::string> faces);

	void setModelMatrix(glm::mat4 const &m);
	glm::mat4 getModelMatrix() const;
	void update(float time);
    void render(RenderData &data);

private:
	GLuint vao_;
	GLuint vertices_VBOSkybox_;
	GLuint program_;
	GLuint cubemapTexture_;
};