#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>

namespace Core {

	class ShaderLoader {
	public:
		ShaderLoader();
		virtual ~ShaderLoader();

		GLuint CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);

		void DeleteProgram(GLuint program);

	private:
		std::string ReadShader(const char *filename);
		GLuint CreateShader(GLenum shaderType, std::string source, const char* shaderName);
	};

}