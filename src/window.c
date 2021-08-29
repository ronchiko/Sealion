
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <SDL/SDL.h>


#include "Sealion/window.h"
#include "Sealion/matrix.h"

struct __seal_context {
	void *sdl_window;
	void *gl_context;

	Seal_Int2 window_size;
	Seal_Matrix2x2 matrix;
	SDL_bool is_active;
};

const Seal_Int2 *WINDOW_SIZE = NULL;
const Seal_Context *CURRENT_CONTEXT = NULL;

static SDL_bool _SDL_initialized = SDL_FALSE;

static void _Seal_Calculate_TranslationMatrix(Seal_Context *ctx) {
	ctx->matrix[0] = 1.f / ctx->window_size.x;
	ctx->matrix[1] = 0;
	ctx->matrix[2] = 0;
	ctx->matrix[3] = 1.f / ctx->window_size.y;
}

static int SDLCALL _Seal_HandleWindowResize(void *data, SDL_Event *event) {
	if(event->type == SDL_WINDOWEVENT) {
		SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
		Seal_Context *context = (Seal_Context *)data;
		
		if(window != context->sdl_window) return 0;  // Not event for this window, exit
		
		switch (event->window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			context->window_size.x = event->window.data1;
			context->window_size.y = event->window.data2;

			_Seal_Calculate_TranslationMatrix(context);
			break;
		case SDL_WINDOWEVENT_CLOSE:
			SDL_DestroyWindow(context->sdl_window);
			context->sdl_window = NULL;
			context->window_size.x = context->window_size.y = 0;
			context->is_active = SDL_FALSE;
			break;
		default: return 0;
		}
		return 0;
	}
	return 0;
}

static void _Seal_End() {
	SDL_Quit();
}

static SDL_bool _Seal_BeginSDL(void) {
	if(_SDL_initialized) return SDL_TRUE;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return SDL_FALSE;
	}

	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	atexit(&_Seal_End);

	return _SDL_initialized = SDL_TRUE;
}

Seal_Context *Seal_NewContext(const char* title, size_t w, size_t h) {

	if(!_Seal_BeginSDL()) return NULL;

	SDL_Window *_window = SDL_CreateWindow(title, 	
										   SDL_WINDOWPOS_UNDEFINED, 
										   SDL_WINDOWPOS_UNDEFINED,
										   w, h,
										   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext gl_context = SDL_GL_CreateContext(_window);

	GLenum successful = glewInit();
	if(successful != GLEW_OK) {
		printf("Failed to initialize GLEW %s", glGetError());
		return NULL;
	}


	Seal_Context *_context = malloc(sizeof(Seal_Context));
	_context->gl_context = gl_context;
	_context->sdl_window = _window;
	_context->window_size.x = w; 
	_context->window_size.y = h;
	_context->is_active = SDL_TRUE;
	_Seal_Calculate_TranslationMatrix(_context);

	SDL_AddEventWatch(_Seal_HandleWindowResize, _context);

	Seal_SwapContext(_context);
	
	return _context;
}

int Seal_SwapContext(Seal_Context *ctx) {
	if(!ctx || !ctx->is_active) return 0;

	SDL_GL_MakeCurrent(ctx->sdl_window, ctx->gl_context);
	CURRENT_CONTEXT = ctx;
	WINDOW_SIZE = &ctx->window_size;
	glViewport(0, 0, ctx->window_size.x, ctx->window_size.y);
	
	return ctx->is_active;
}

void Seal_DestroyContext(Seal_Context *ctx) {
	if(!ctx) return;

	SDL_DestroyWindow(ctx->sdl_window);
	SDL_GL_DeleteContext(ctx->gl_context);
	SDL_DelEventWatch(_Seal_HandleWindowResize, ctx);

	// Discard the context if its public context
	if(CURRENT_CONTEXT == ctx) 
		CURRENT_CONTEXT = NULL;

	if(WINDOW_SIZE == &ctx->window_size)
		WINDOW_SIZE = NULL;

	free(ctx);
}

void Seal_BeginPipelines(Seal_RenderPipeline pipelines[], int count) {
	
	SDL_Event event;
	int alive_contextes = count;
	for(;alive_contextes > 0;) {
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) break;
		}
		
		Seal_ContextData _contextdata;

		alive_contextes = 0;
		for(Seal_RenderPipeline *pipeline = pipelines; pipeline < pipelines + count; ++pipeline) {
			
			_contextdata.size = pipeline->context->window_size;
			memcpy(_contextdata.matrix, pipeline->context->matrix, sizeof(Seal_Matrix2x2));

			if(!Seal_SwapContext(pipeline->context)) continue;

			pipelines->pipeline(&_contextdata, pipelines->data);

			SDL_GL_SwapWindow(pipeline->context->sdl_window);
			alive_contextes++;
		}
	}
}