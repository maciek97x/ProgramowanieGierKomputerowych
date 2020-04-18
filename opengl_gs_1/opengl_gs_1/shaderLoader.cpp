#include "ShaderLoader.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace Core;

ShaderLoader::ShaderLoader() {}
ShaderLoader::~ShaderLoader() {}

std::string ShaderLoader::ReadShader(const char *filename) {

	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good()) {
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string source, const char* shaderName) {

	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//sprawdz bledy
	if (compile_result == GL_FALSE) {

		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}

	return shader;
}

GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename) {

	//wczytaj shadery
	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");

	int link_result = 0;
	//stworz shader
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//sprawdz bledy w linkerze
	if (link_result == GL_FALSE) {

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename) {

	std::string vertex_shader_code = ReadShader(vertexShaderFilename);
	std::string geometry_shader_code = ReadShader(geometryShaderFilename);
	std::string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
	GLuint geometry_shader = CreateShader(GL_GEOMETRY_SHADER, geometry_shader_code, "geometry shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");

	int link_result = 0;
	//stworz shader
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//sprawdz bledy w linkerze
	if (link_result == GL_FALSE) {

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}

	glDetachShader(program, vertex_shader);
	glDetachShader(program, geometry_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(geometry_shader);
	glDeleteShader(vertex_shader);

	return program;
}

void ShaderLoader::DeleteProgram(GLuint program) {
	glDeleteProgram(program);
}
