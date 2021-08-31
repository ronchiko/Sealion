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

extern void _Seal_DrawMatrix(float *matrix, int w, int h);

struct _packed_pipeline_data_ {
	Seal_GL_Program program;
	Seal_Batcher *batcher;
	Seal_Sprite sprite1, sprite2;	
};

void _Basic_Pipeline(Seal_ContextData *ctx, void *raw_data) {
	
	static float xspeed = 0.2f, yspeed = 0.2f, camerax = 0.f;

	GL_SAFE(glClear(GL_COLOR_BUFFER_BIT));
	struct _packed_pipeline_data_ *data = raw_data;
	
	glUseProgram(data->program);

	static Seal_Camera camera = {
		{0, 0}, 2.f, 1.f, 100.f
	};
	float ratio = (float)ctx->size.x / ctx->size.y;

	Seal_Matrix3x3 screen;
	Seal_M3Camera(screen, &camera, ratio);
	Seal_M3Transpose(screen);

	GLuint _half = glGetUniformLocation(data->program, "_translation");
	glUniformMatrix3fv(_half, 1, GL_FALSE, screen);
	
	Seal_BatchPush(data->batcher, &data->sprite1);

	Seal_DrawBatch(data->batcher);
}

int main() {
	Seal_Context *ctx = Seal_NewContext("Dodger", WIDTH, HEIGHT);

	// OpenGL initialize, TODO: Should be moved to a function
	Seal_GL_Shader vertex = Seal_ShaderFromSource("VERTEX_DEFAULT", DEFAULT_VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER);
	Seal_GL_Shader fragment = Seal_ShaderFromSource("FRAGMENT_DEFAULT", DEFAULT_FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER);

	Seal_GL_Program program = 0;
	if(vertex && fragment)
		program = Seal_MakeProgram("DEFAULT", vertex, fragment);
	

	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WIDTH, HEIGHT);

	Seal_Batcher *batcher = Seal_NewBatcher(program);
	Seal_Sprite sprite1 = { {10.f, 0.f }, { 10.f, 10.f }, 0, 0, { 1.f, 0, 0, 1 }}; 
	Seal_Sprite sprite2 = { {0, 1 }, { 50, 50 }, 0, 0, { 0, 1.f, 1, 1 }}; 

	struct _packed_pipeline_data_ pipeline_data;
	pipeline_data.program = program;
	pipeline_data.batcher = batcher;
	pipeline_data.sprite1 = sprite1;
	pipeline_data.sprite2 = sprite2;

	Seal_RenderPipeline pipeline = {
		ctx, &_Basic_Pipeline, &pipeline_data
	};

	Seal_BeginPipeline(pipeline);

	Seal_DestroyBatcher(batcher);
	Seal_DestroyContext(pipeline.context);

	return 0;
}