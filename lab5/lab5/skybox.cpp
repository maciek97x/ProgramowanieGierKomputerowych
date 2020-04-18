#include "Skybox.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}
bool SkyBox::init(std::vector<std::string> faces) {
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenTextures(1, &cubemapTexture_);
	glActiveTexture(GL_TEXTURE0);
	cubemapTexture_ = SOIL_load_OGL_cubemap(
		faces[0].c_str(),
		faces[1].c_str(),
		faces[2].c_str(),
		faces[3].c_str(),
		faces[4].c_str(),
		faces[5].c_str(),
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	Core::ShaderLoader shaderLoader = Core::ShaderLoader();

	const char * skyboxShaderVert = "shaders/shader_skybox.vert";
	const char * skyboxShaderFrag = "shaders/shader_skybox.frag";
	program_ = shaderLoader.CreateProgram(skyboxShaderVert, skyboxShaderFrag);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glBindVertexArray(0);
	return true;
}

void SkyBox::setModelMatrix(glm::mat4 const & m)
{
	// ignore (skybox is static)
}

glm::mat4 SkyBox::getModelMatrix() const
{
	return glm::mat4();
}

void SkyBox::update(float time)
{
}

void SkyBox::render(RenderData& data) {
	glm::mat4 viewNew = glm::mat4(glm::mat3(data.viewMatrix)); // Remove translation components

	glDepthFunc(GL_LEQUAL); // z = 1.0 in skybox, the default GL_LESS would discard the skybox

	glUseProgram(program_);
	glUniformMatrix4fv(glGetUniformLocation(program_, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewNew));
	glUniformMatrix4fv(glGetUniformLocation(program_, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(data.projMatrix));

	// Standard cube rendering
	glBindVertexArray(vao_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_); // GL_TEXTURE_CUBE_MAP instead of 2D!
	glUniform1i(glGetUniformLocation(program_, "skybox"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);

	// reset depth func
	glDepthFunc(GL_LESS);
	glUseProgram(0);
}
