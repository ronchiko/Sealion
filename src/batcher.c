
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>

#include "Sealion/graphics.h"

#define DLL_EXPORT __declspec(dllexport)

#define BATCHER_DEFAULT_SIZE 1024

#define SPRITE_TOP	 	(_sprite->position.y - _sprite->scale.x * 0.5f)
#define SPRITE_LEFT 	(_sprite->position.x - _sprite->scale.y * 0.5f)
#define SPRITE_RIGHT	(_sprite->position.x + _sprite->scale.x * 0.5f)
#define SPRITE_BOTTOM	(_sprite->position.y + _sprite->scale.y * 0.5f)

#define _X_TRANSLATE(in_x) in_x // (((float)(in_x)) / (WINDOW_SIZE->x >> 1))
#define _Y_TRANSLATE(in_y) in_y // (((float)(in_y)) / (WINDOW_SIZE->y >> 1))

#define GL_SAFE(op) {op; GLenum error = glGetError(); if(error != GL_NO_ERROR) \
	{ printf("ERROR at "#op": %s\n", gluErrorString(error)); exit(-1); } } 

Seal_Batcher *Seal_NewBatcher(void) {
	Seal_Batcher *batcher = malloc(sizeof(Seal_Batcher));

	batcher->max_vertecies = BATCHER_DEFAULT_SIZE;
	batcher->vertecies = malloc(batcher->max_vertecies * sizeof(Seal_Rendering_Vertex));
	batcher->used_vertecies = 0;
	batcher->indecies = 0;
	
	GL_SAFE(glCreateVertexArrays(1, &batcher->vertex_object));
	GL_SAFE(glBindVertexArray(batcher->vertex_object));
	
	GL_SAFE(glGenBuffers(1, &batcher->vertex_buffer));
	GL_SAFE(glBindBuffer(GL_ARRAY_BUFFER, batcher->vertex_buffer));
	GL_SAFE(glBufferData(GL_ARRAY_BUFFER, BATCHER_DEFAULT_SIZE * sizeof(Seal_Rendering_Vertex), NULL, GL_DYNAMIC_DRAW));

	GL_SAFE(glEnableVertexArrayAttrib(batcher->vertex_buffer, 0));
	GL_SAFE(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Seal_Rendering_Vertex), (const void*)offsetof(Seal_Rendering_Vertex, position)));

	GL_SAFE(glEnableVertexArrayAttrib(batcher->vertex_buffer, 2));
	GL_SAFE(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Seal_Rendering_Vertex), (const void*)offsetof(Seal_Rendering_Vertex, tint)));

	GL_SAFE(glGenBuffers(1, &batcher->index_buffer));
	unsigned int indecies[BATCHER_DEFAULT_SIZE * 6];

	for(int i = 0; i < BATCHER_DEFAULT_SIZE; i++) {
		
		indecies[6 * i + 0] = i * 4 + 0;
		indecies[6 * i + 1] = i * 4 + 1;
		indecies[6 * i + 2] = i * 4 + 2;

		indecies[6 * i + 3] = i * 4 + 0;
		indecies[6 * i + 4] = i * 4 + 3;
		indecies[6 * i + 5] = i * 4 + 2;
	}

	GL_SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batcher->index_buffer));
	GL_SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, BATCHER_DEFAULT_SIZE * 6 * sizeof(int), indecies, GL_STATIC_DRAW));	

	return batcher;
}

void Seal_BatchPush(Seal_Batcher *_batcher, Seal_Sprite *_sprite) {
	
	if(_batcher->used_vertecies + 4 >= _batcher->max_vertecies) return; // Can't push

	float left 		= (float)_X_TRANSLATE(SPRITE_LEFT);
	float right 	= (float)_X_TRANSLATE(SPRITE_RIGHT);

	float top 		= (float)_Y_TRANSLATE(SPRITE_TOP);
	float bottom 	= (float)_Y_TRANSLATE(SPRITE_BOTTOM);

	float ratio = ((float)WINDOW_SIZE->x) / WINDOW_SIZE->y;
	float inv_r = 1.f / ratio;

	_batcher->vertecies[_batcher->used_vertecies++] = 
		(Seal_Rendering_Vertex){ {left , top   }, {0, 1}, _sprite->texture, _sprite->color };
	_batcher->vertecies[_batcher->used_vertecies++] = 
		(Seal_Rendering_Vertex){ {left , bottom}, {0, 0}, _sprite->texture, _sprite->color };
	_batcher->vertecies[_batcher->used_vertecies++] = 
		(Seal_Rendering_Vertex){ {right, bottom}, {1, 0}, _sprite->texture, _sprite->color };
	_batcher->vertecies[_batcher->used_vertecies++] = 
		(Seal_Rendering_Vertex){ {right, top   }, {1, 1}, _sprite->texture, _sprite->color };

	_batcher->indecies += 6;
}

void Seal_DrawBatch(Seal_Batcher *_batcher) {

	GL_SAFE(glBindBuffer(GL_ARRAY_BUFFER, _batcher->vertex_buffer));
	GL_SAFE(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Seal_Rendering_Vertex) * _batcher->used_vertecies, _batcher->vertecies));

	GL_SAFE(glBindVertexArray(_batcher->vertex_object));
	GL_SAFE(glDrawElements(GL_TRIANGLES, _batcher->indecies, GL_UNSIGNED_INT, NULL));

	_batcher->indecies = 0;
	_batcher->used_vertecies = 0;
}

void Seal_DestroyBatcher(Seal_Batcher *_batcher) {
	if(!_batcher) return;
	
	glDeleteBuffers(2, &_batcher->vertex_buffer);
	glDeleteVertexArrays(1, &_batcher->vertex_object);
	
	free(_batcher->vertecies);
	free(_batcher);
}

void Seal_DiscardBatcher(Seal_Batcher *_batcher) {
	if(!_batcher) return;

	free(_batcher->vertecies);
	free(_batcher);
}