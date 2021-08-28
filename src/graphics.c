
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include "Sealion/graphics.h"

Seal_GL_Shader Seal_ShaderFromSource(const char* _name, 
							const char *_source, 
							GLenum _shader_type) {
	
	Seal_GL_Shader shader = glCreateShader(_shader_type);

	glShaderSource(shader, 1, (const GLchar **)&_source, NULL);
	glCompileShader(shader);

	GLint compiled_successful = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled_successful);
	if(compiled_successful != GL_TRUE) {
		printf("Failed to compile shader '%s':\n", _name);

		GLint messageLength, logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		char *message = malloc(sizeof(GLchar) * logLength);
		glGetShaderInfoLog(shader, logLength, &messageLength, message);
		if(messageLength > 0){
			printf("%s\n", message);
		}

		glDeleteShader(shader);

		return 0;
	}

	return shader;
}

Seal_GL_Program Seal_MakeProgram(const char *_name,
					   Seal_GL_Shader _vertex, 
					   Seal_GL_Shader _fragment) {

	Seal_GL_Program program = glCreateProgram();

	glAttachShader(program, _vertex);
	glAttachShader(program, _fragment);

	glLinkProgram(program);
	glValidateProgram(program);

	GLint link_status = GL_FALSE;
	
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if(link_status != GL_TRUE) {
		printf("Failed to link program %s: ", _name);

		GLuint messageSize, logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		GLchar *message = malloc(sizeof(GLchar) * logLength);
		glGetProgramInfoLog(program, logLength, &messageSize, message);

		if(messageSize > 0){
			printf("%s\n", message);
		}

		glDeleteProgram(program);
		return 0;
	}

	return program;
} 
