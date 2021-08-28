#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <SDL/SDL.h>

#include "Sealion.h"

#include "resources/embedded.h"

#define GL_SAFE(op) {op; GLenum error = glGetError(); if(error != GL_NO_ERROR) \
	{ printf("ERROR at "#op": %s\n", gluErrorString(error)); exit(-1); } } 

#define WIDTH 600
#define HEIGHT 400

struct _packed_pipeline_data_ {
	Seal_GL_Program program;
	Seal_Batcher *batcher;
	Seal_Sprite sprite1, sprite2;	
};

void _Basic_Pipeline(Seal_ContextData *ctx, void *raw_data) {
	
	GL_SAFE(glClear(GL_COLOR_BUFFER_BIT));
	struct _packed_pipeline_data_ *data = raw_data;
	
	glUseProgram(data->program);

	GLuint _half = glGetUniformLocation(data->program, "_translation");
	glUniformMatrix2fv(_half, 1, GL_FALSE, ctx->matrix);
	
	Seal_BatchPush(data->batcher, &data->sprite1);
	Seal_BatchPush(data->batcher, &data->sprite2);

	Seal_DrawBatch(data->batcher);
}

int main() {

	Seal_RenderPipeline pipelines[2] = { 0 };
	for(int i = 0; i < 2; ++i) {
		Seal_Context *ctx = Seal_NewContext("Dodger", WIDTH, HEIGHT);

		// OpenGL initialize, TODO: Should be moved to a function
		Seal_GL_Shader vertex = Seal_ShaderFromSource("VERTEX_DEFAULT", DEFAULT_VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER);
		Seal_GL_Shader fragment = Seal_ShaderFromSource("FRAGMENT_DEFAULT", DEFAULT_FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER);

		Seal_GL_Program program = 0;
		if(vertex && fragment)
			program = Seal_MakeProgram("DEFAULT", vertex, fragment);
		

		glClearColor(0, 0, 0, 1);
		glViewport(0, 0, WIDTH, HEIGHT);

		Seal_Batcher *batcher = Seal_NewBatcher();
		Seal_Sprite sprite1 = { {0, 0 }, { 100, 100 }, 0, 0, { 1.f, 0, 0, 1 }}; 
		Seal_Sprite sprite2 = { {0, 1 }, { 50, 50 }, 0, 0, { 0, 1.f, 1, 1 }}; 

		struct _packed_pipeline_data_ *pipeline_data = malloc(sizeof(struct _packed_pipeline_data_));
		pipeline_data->program = program;
		pipeline_data->batcher = batcher;
		pipeline_data->sprite1 = sprite1;
		pipeline_data->sprite2 = sprite2;

		Seal_RenderPipeline pipeline = {
			ctx, &_Basic_Pipeline, pipeline_data
		};

		memcpy(pipelines + i, &pipeline, sizeof(Seal_RenderPipeline));  
	}

	Seal_BeginPipelines(pipelines, 2);

	for(int i = 0; i < 2; i++) {

		struct _packed_pipeline_data_ *data = pipelines[i].data;

		Seal_DestroyBatcher(data->batcher);
		Seal_DestroyContext(pipelines[i].context);
		free(data);
	}


	return 0;
}