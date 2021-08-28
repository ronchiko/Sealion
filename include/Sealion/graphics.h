#pragma once

#include "Sealion/vector2.h"
#include "Sealion/color.h"

#include <stddef.h>
#include <GL/glew.h> 

typedef unsigned int texture_id_t; 

typedef unsigned int Seal_GL_Shader;
typedef unsigned int Seal_GL_Program;
typedef unsigned int Seal_GL_Buffer;

extern const Seal_Int2 *WINDOW_SIZE;

typedef struct {
	Seal_Vec2 position, scale;
	float angle;
	texture_id_t texture;
	Seal_Color color;
} Seal_Sprite;

Seal_GL_Shader Seal_ShaderFromSource(const char *name, const char *source, GLenum shader_type);
Seal_GL_Program Seal_MakeProgram(const char *name, Seal_GL_Shader Seal_Rendering_Vertex, Seal_GL_Shader fragment);

typedef struct {
	Seal_Vec2 position;
	Seal_Vec2 uv;
	int texture;
	Seal_Color tint;
} Seal_Rendering_Vertex;

typedef struct {
	Seal_GL_Buffer vertex_buffer;
	Seal_GL_Buffer index_buffer;
	Seal_GL_Buffer vertex_object;

	Seal_Rendering_Vertex *vertecies;

	size_t max_vertecies;
	size_t indecies;
	size_t used_vertecies;
} Seal_Batcher;

Seal_Batcher *Seal_NewBatcher(void);
void Seal_BatchPush(Seal_Batcher *batcher, Seal_Sprite *Seal_Sprite);
/* Draw the batch*/
void Seal_DrawBatch(Seal_Batcher *batcher);
/* Destroy the buffer completly */
void Seal_DestroyBatcher(Seal_Batcher *batcher);
/* Destroy the buffer and all its allocated memory, but doesn't clean GL allocated object (VBO, etc...) */
void Seal_DiscardBatcher(Seal_Batcher *batcher);